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
	double x, y; // position
	double vx, vy; // velocity
};

struct Point {
	double x, y;
};

// constants
const int ballRadius = 20;
const int screenW = 600;
const int screenH = 400;
const double maxForce = 10;
const double minForce = 0;
const double maxAngle = 90;
const double minAngle = -90;
const double forceIncrement = (maxForce - minForce) / 100.0;
const double angleIncrement = (maxAngle - minAngle) / 180.0;
const int guideLength = 100;
const double PI = 3.1415926535897932384626433832795;
const double velocityThreshold = maxForce / 100;
const double friction = 0.02;

// globals
double force = maxForce / 2;
double launchAngle = 0;
double launchAngleRadians = 0;
double currentAngle = 0;
boolean guideVisible = true;
int del = 0;

Ball cue = { ballRadius, ballRadius, screenH / 2, 0, 0 };
Ball eight = { ballRadius, screenW / 2, screenH / 2, 0, 0 };

void print(Ball b) {
	std::cout << "pos: (" << b.x << ", " << b.y << ") vel: (" << b.vx << ", " << b.vy << ")\n";
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
	glRectd(left, bot, right, (((double) top - (double) bot) * percentForce + bot));
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
	drawCircle(cue.x, cue.y, cue.radius);
}

void drawEightBall(void) {
	color255(BLACK);
	drawCircle(eight.x, eight.y, eight.radius);
}

void drawGuide(void) {
	if (guideVisible) {
		color255(WHITE);
		glBegin(GL_LINE_STRIP);
		glVertex2d(cue.x, cue.y);
		glVertex2d(cue.x + guideLength * cos(launchAngleRadians), cue.y + guideLength * sin(launchAngleRadians));
		glEnd();
	}
}

int isCollision() {
	double distance;
	distance = sqrt(pow(cue.x - eight.x, 2) + pow(cue.y - eight.y, 2));
	return distance <= ((double) cue.radius + (double) eight.radius);
}

void checkWallCollision(Ball *ball) {
	if ((ball->x - ball->radius) < 0) {
		ball->x = ball->radius;
		ball->vx = -ball->vx;
	}
	if ((ball->x + ball->radius) > screenW) {
		ball->x = (double) screenW - (double) ball->radius;
		ball->vx = -ball->vx;
	}
	if ((ball->y - ball->radius) < 0) {
		ball->y = ball->radius;
		ball->vy = -ball->vy;
	}
	if ((ball->y + ball->radius) > screenH) {
		ball->y = (double) screenH - (double) ball->radius;
	}
}

double velo(Ball *ball) {
	return sqrt(pow(ball->vx, 2) + pow(ball->vy, 2));
}

Point ballCollision(Ball* moved, Ball* still) {
	Point vector = { moved->x - still->x, moved->y - still->y };
	double magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	vector.x /= magnitude;
	vector.y /= magnitude;
	vector.x *= velo(moved);
	vector.y *= velo(moved);
	return vector;
}

int moveBalls(void) {
	double slope;

	cue.x += cue.vx;
	cue.y += cue.vy;
	eight.x += eight.vx;
	eight.y += eight.vy;

	if (isCollision()) {
		Point cueNewV = ballCollision(&cue, &eight);
		Point eightNewV = ballCollision(&eight, &cue);
		cue.vx = cueNewV.x;
		cue.vy = cueNewV.y;
		eight.vy = eightNewV.y;
		eight.vy = eightNewV.y;
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