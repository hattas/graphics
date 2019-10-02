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
	GLfloat angle; // angle ball is traveling in
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

Ball cue = { ballRadius, ballRadius, screenH, 0, 0, 0 };
Ball eight = { ballRadius, screenW / 2, screenH / 2, 0, 0, 0 };

void color255(int r, int g, int b) {
	glColor3f((GLfloat)(r / 255.0), (GLfloat)(g / 255.0), (GLfloat)(b / 255.0));
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
	glRectf(left, bot, right, ((top - bot) * percentForce + bot));
}

void drawPoolTable(void) {
	color255(GREEN);
	glRectf(0, 0, screenW, screenH);
}

void drawCircle(GLfloat x, GLfloat y, int radius) {
	glBegin(GL_POLYGON);
	for (GLfloat i = 0; i < 2 * PI; i += PI / 40)
		glVertex2f(x + (GLfloat)cos(i) * radius, y + (GLfloat)sin(i) * radius);
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
		glVertex2f(cue.x, cue.y);
		glVertex2f(cue.x + guideLength * cos(launchAngleRadians), cue.y + guideLength * sin(launchAngleRadians));
		glEnd();
	}
}

boolean moveBalls(void) {
	// adjust ball positions
	cue.x += cue.vx;
	cue.y += cue.vy;

	// check for collisions with the walls
	if ((cue.x - cue.radius) < 0) {
		cue.x = cue.radius;
		cue.vx = -cue.vx;
	}
	if ((cue.x + cue.radius) > screenW) {
		cue.x = screenW - cue.radius;
		cue.vx = -cue.vx;
	}
	if ((cue.y - cue.radius) < 0) {
		cue.y = cue.radius;
		cue.vy = -cue.vy;
	}
	if ((cue.y + cue.radius) > screenH) {
		cue.y = screenH - cue.radius;
		cue.vy = -cue.vy;
	}

	if (cue.vx != 0) {
		currentAngle = atan(cue.vy / cue.vy);
		cue.vx -= friction * abs(cos(currentAngle));
		cue.vy -= friction * abs(sin(currentAngle));
	}
	else if (cue.vy > 0) {
		cue.vy -= friction;
	}

	// check for small velocity and set to 0
	GLfloat velocityMagnitude = abs(sqrt(cue.vx * cue.vx + cue.vy * cue.vy));
	if (velocityMagnitude < velocityThreshold) {
		cue.vx = 0;
		cue.vy = 0;
	}

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
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Pool Balls Simulation");

	// register the callback functions
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(mySpecialKeyboard);
	glutKeyboardFunc(myKeyboard);

	myInit();
	glutMainLoop();
}