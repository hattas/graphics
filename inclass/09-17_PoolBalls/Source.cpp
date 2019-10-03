#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>

// colors from https://flatuicolors.com/palette/au
#define RED 235, 77, 75
#define BLACK 50, 50, 50
#define WHITE 255, 255, 255
#define CUE_WHITE 255, 255, 237
#define GREEN 104, 207, 54
#define BROWN 240, 147, 43

struct Ball {
	int radius;
	double px, py; // position
	double vx, vy; // velocity
	double ax, ay;
};

// constants
const int ballRadius = 20;
const int screenW = 600;
const int screenH = 400;
const double maxForce = 30;
const double minForce = 0;
const double maxAngle = 90;
const double minAngle = -90;
const double forceIncrement = (maxForce - minForce) / 100.0;
const double angleIncrement = (maxAngle - minAngle) / 180.0;
const int guideLength = 100;
const double PI = 3.1415926535897932384626433832795;
const double velocityThreshold = maxForce / 100;
const double friction = 0.01;

// globals
double force = (maxForce + minForce) / 2;
double launchAngle = 0;
double launchAngleRadians = 0;
double currentAngle = 0;
boolean guideVisible = true;
int del = 0;

Ball cue = { ballRadius, ballRadius, screenH / 2, 0, 0, 0, 0 };
Ball eight = { ballRadius, screenW / 2, screenH / 2, 0, 0, 0, 0 };

void print(Ball b) {
	std::cout << "pos: (" << b.px << ", " << b.py << ") vel: (" << b.vx << ", " << b.vy << ") acc: (" << b.ax << ", " << b.ay << ")\n";
}

void color255(int r, int g, int b) {
	glColor3d((GLdouble)(r / 255.0), (GLdouble)(g / 255.0), (GLdouble)(b / 255.0));
}

void myInit(void) {
	glClearColor(0, 0, 0, 0);
	color255(WHITE);
	glPointSize(1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screenW, 0.0, screenH);
}

void drawForceBar(void) {
	color255(WHITE);
	int left = screenW / 16;
	int right = screenW / 8;
	int bot = screenH / 8;
	int top = screenH / 8 * 7;
	glRecti(left, bot, right, top);

	color255(RED);
	double percentForce = force / maxForce;
	glRectd(left, bot, right, (((double)top - (double)bot) * percentForce + bot));
}

void drawPoolTable(void) {
	color255(GREEN);
	glRectf(0, 0, screenW, screenH);
}

void drawCircle(double x, double y, int radius) {
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * PI; i += PI / 40)
		glVertex2d(x + cos(i) * radius, y + sin(i) * radius);
	glEnd();
}

void drawCueBall(void) {
	color255(CUE_WHITE);
	drawCircle(cue.px, cue.py, cue.radius);
}

void drawEightBall(void) {
	color255(BLACK);
	drawCircle(eight.px, eight.py, eight.radius);
}

void drawGuide(void) {
	if (guideVisible) {
		color255(WHITE);
		glBegin(GL_LINE_STRIP);
		glVertex2d(cue.px, cue.py);
		glVertex2d(cue.px + guideLength * cos(launchAngleRadians), cue.py + guideLength * sin(launchAngleRadians));
		glEnd();
	}
}

int isCollision() {
	double distance;
	distance = pow(cue.px - eight.px, 2) + pow(cue.py - eight.py, 2);
	return distance <= pow((double)cue.radius + (double)eight.radius, 2);
}

void checkWallCollision(Ball* ball) {
	if ((ball->px - ball->radius) < 0) {
		ball->px = ball->radius;
		ball->vx = -ball->vx;
	}
	if ((ball->px + ball->radius) > screenW) {
		ball->px = (double)screenW - (double)ball->radius;
		ball->vx = -ball->vx;
	}
	if ((ball->py - ball->radius) < 0) {
		ball->py = ball->radius;
		ball->vy = -ball->vy;
	}
	if ((ball->py + ball->radius) > screenH) {
		ball->py = (double)screenH - (double)ball->radius;
		ball->vy = -ball->vy;
	}
}

double velo(Ball* ball) {
	return sqrt(pow(ball->vx, 2) + pow(ball->vy, 2));
}

int moveBalls(void) {
	double distance, overlap, nx, ny, kx, ky, p;
	char collision;

	// update ball positions
	cue.ax = -cue.vx * friction;
	cue.ay = -cue.vy * friction;
	cue.px += cue.vx;
	cue.py += cue.vy;
	cue.vx += cue.ax;
	cue.vy += cue.ay;
	eight.ax = -eight.vx * friction;
	eight.ay = -eight.vy * friction;
	eight.px += eight.vx;
	eight.py += eight.vy;
	eight.vx += eight.ax;
	eight.vy += eight.ay;

	// cutoff velo close to 0
	if (abs(cue.vx * cue.vx + cue.vy * cue.vy) < 0.01) {
		cue.vx = 0;
		cue.vy = 0;
	}
	if (abs(eight.vx * eight.vx + eight.vy * eight.vy) < 0.01) {
		eight.vx = 0;
		eight.vy = 0;
	}

	collision = isCollision();
	
	if (collision) {
		// resolve static collision preventing balls from being inside each other
		distance = sqrt(pow(cue.px - eight.px, 2) + pow(cue.py - eight.py, 2));
		overlap = 0.5 * (distance - cue.radius - eight.radius);
		
		// move cue away from collision
		cue.px -= overlap * (cue.px - eight.px) / distance;
		cue.py -= overlap * (cue.py - eight.py) / distance;

		// move eight ball away from collision
		eight.px += overlap * (cue.px - eight.px) / distance;
		eight.py += overlap * (cue.py - eight.py) / distance;

		// resolve dynamic collision
		// https://en.wikipedia.org/wiki/Elastic_collision
		// section called Two-dimensional collision with two moving objects
		distance = sqrt(pow(cue.px - eight.px, 2) + pow(cue.py - eight.py, 2));
		nx = (eight.px - cue.px) / distance;
		ny = (eight.py - cue.py) / distance;
		kx = cue.vx - eight.vx;
		ky = cue.vy - eight.vy;
		p = nx * kx + ny * ky;
		cue.vx = cue.vx - p * nx;
		cue.vy = cue.vy - p * ny;
		eight.vx = eight.vx + p * nx;
		eight.vy = eight.vy + p * ny;
	}

	checkWallCollision(&cue);
	checkWallCollision(&eight);

	return true;
}

void myDisplay(void) {
	// set viewport to entire window and draw the force bar
	glViewport(0, 0, screenW, screenH);
	drawForceBar();

	// set viewport to the pool table and draw the pool table scene
	GLsizei tableH = 3 * screenH / 4;
	GLsizei tableW = tableH / 2 * 3;
	glViewport(3 * screenW / 16, screenH / 8, tableW, tableH);
	drawPoolTable();
	drawGuide();
	drawCueBall();
	drawEightBall();

	glutSwapBuffers();

	// move the balls, and if they moved then redisplay
	if (moveBalls())
		glutPostRedisplay();
}

void mySpecialKeyboard(int theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case GLUT_KEY_UP: // force up
		force += forceIncrement;
		if (force > maxForce)
			force = maxForce;
		break;
	case GLUT_KEY_DOWN: // force down
		force -= forceIncrement;
		if (force < minForce)
			force = minForce;
		break;
	case GLUT_KEY_LEFT: // angle up
		launchAngle += angleIncrement;
		if (launchAngle > maxAngle)
			launchAngle = maxAngle;
		break;
	case GLUT_KEY_RIGHT: // angle down
		launchAngle -= angleIncrement;
		if (launchAngle < minAngle)
			launchAngle = minAngle;
		break;
	}
	launchAngleRadians = (launchAngle * PI) / 180;
	std::cout << force << ' ' << launchAngle << '\n';
	glutPostRedisplay();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case ' ':
		cue.vx = force * cos(launchAngleRadians);
		cue.vy = force * sin(launchAngleRadians);
		guideVisible = false;
		break;
	case 'r':
		cue.px = cue.radius;
		cue.py = screenH / 2;
		cue.vx = 0;
		cue.vy = 0;
		cue.ax = 0;
		cue.ay = 0;
		eight.px = screenW / 2;
		eight.py = screenH / 2;
		eight.vx = 0;
		eight.vy = 0;
		eight.ax = 0;
		eight.ay = 0;
		launchAngle = 0;
		launchAngleRadians = 0;
		force = (maxForce + minForce) / 2;
		guideVisible = true;
		break;
	default: break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(400, 200);
	glutCreateWindow("Pool Balls Simulation");

	// register the callback functions
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(mySpecialKeyboard);
	glutKeyboardFunc(myKeyboard);

	myInit();
	glutMainLoop();
}