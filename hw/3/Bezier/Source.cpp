#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>

struct Point {
	GLfloat x, y;
};

// constants
const int numPoints = 4;
const int screenW = 400;
const int screenH = 400;
const double tInitial = 0;
const double tFinal = 1;
const int numTSteps = 100;
const double stepSize = (tFinal - tInitial) / numTSteps;

// globals
Point points[numPoints];
int ixAnimation = 0; // frame number of animation

void myInit(void) {
	glClearColor(1, 1, 1, 0);
	glColor3f(1, 1, 1);
	glPointSize(4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screenW, 0.0, screenH);

	// hardcode bezier curve points
	points[0] = { 50, 50 };
	points[1] = { 100, 300 };
	points[2] = { 200, 300 };
	points[3] = { 350, 50 };
}

void pointsLoop(void) {
	for (int i = 0; i < numPoints; i++)
		glVertex2f(points[i].x, points[i].y);
}

void drawFourPoints(void) {
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POINTS);
	pointsLoop();
	glEnd();
	glBegin(GL_LINE_STRIP);
	pointsLoop();
	glEnd();
}

void myDisplay(void) {
	// don't go past end of animation
	if (ixAnimation > numTSteps)
		ixAnimation = numTSteps;

	glClear(GL_COLOR_BUFFER_BIT);

	drawFourPoints();

	// bezier equation:
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	double tMiddle = ixAnimation * stepSize;
	double x, y;
	for (double t = tInitial; t <= tMiddle; t += stepSize) {
		x = pow(1 - t, 3) * points[0].x + 3 * pow(1 - t, 2) * t * points[1].x + 3 * (1 - t) * pow(t, 2) * points[2].x + pow(t, 3) * points[3].x;
		y = pow(1 - t, 3) * points[0].y + 3 * pow(1 - t, 2) * t * points[1].y + 3 * (1 - t) * pow(t, 2) * points[2].y + pow(t, 3) * points[3].y;
		glVertex2f(x, y);
	}
	glEnd();
	ixAnimation++;
	if (ixAnimation <= numTSteps) {
		glutPostRedisplay();
	}

	glutSwapBuffers();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case 'r':
		ixAnimation = 0;
		glutPostRedisplay();
		break;
	default: break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("HW 3 Bezier Animation");

	// register the callback functions
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);

	myInit();
	glutMainLoop();
}