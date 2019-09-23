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
const int screenW = 600;
const int screenH = 600;
const double tInitial = 0;
const double tFinal = 1;
const int numFrames = 200;
const double stepSize = (tFinal - tInitial) / numFrames;
const double lineWidth = 1.5;

// globals
Point p[numPoints]; // control points
Point q[numPoints]; // intermediate linear control points
Point r[numPoints]; // intermediate quadratic control points
int iFrame = 0; // frame number of animation
int iPoint = 0;

void myInit(void) {
	glClearColor(1, 1, 1, 0);
	glColor3f(1, 1, 1);
	glPointSize(6);
	glLineWidth(lineWidth);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screenW, 0.0, screenH);

	// initial control points
	p[0] = { 100, 100 };
	p[1] = { 400, 200 };
	p[2] = { 200, 400 };
	p[3] = { 500, 500 };
}

void pointsLoop(void) {
	for (int i = 0; i < numPoints; i++)
		glVertex2f(p[i].x, p[i].y);
}

void drawControlPoints(void) {
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POINTS);
	pointsLoop();
	glEnd();
	glBegin(GL_LINE_STRIP);
	pointsLoop();
	glEnd();
}

Point linearBezier(Point p0, Point p1, double t) {
	double x, y;
	x = (1 - t) * p0.x + t * p1.x;
	y = (1 - t) * p0.y + t * p1.y;
	Point p = { x, y };
	return p;
}

void getIntermediatePoints(double t) {
	int i;
	for (i = 0; i < numPoints - 1; i++) {
		q[i] = linearBezier(p[i], p[i + 1], t);
	}

	for (i = 0; i < numPoints - 2; i++) {
		r[i] = linearBezier(q[i], q[i + 1], t);
	}
}

void drawGreenLines(void) {
	glColor3f(0.5, 1, 0.5);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numPoints - 1; i++) {
		glVertex2d(q[i].x, q[i].y);
	}
	glEnd();
	glBegin(GL_POINTS);
	for (int i = 0; i < numPoints - 1; i++) {
		glVertex2d(q[i].x, q[i].y);
	}
	glEnd();
}

void drawBlueLines(void) {
	glColor3f(0.5, 0.5, 1);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numPoints - 2; i++) {
		glVertex2d(r[i].x, r[i].y);
	}
	glEnd();
	glBegin(GL_POINTS);
	for (int i = 0; i < numPoints - 2; i++) {
		glVertex2d(r[i].x, r[i].y);
	}
	glEnd();
}

void drawCurve(double t) {
	double x, y;
	glColor3f(1, 0, 0);
	glLineWidth(lineWidth * 2);
	glBegin(GL_LINE_STRIP);
	for (double tt = tInitial; tt <= t; tt += stepSize) {
		x = pow(1 - tt, 3) * p[0].x + 3 * pow(1 - tt, 2) * tt * p[1].x + 3 * (1 - tt) * pow(tt, 2) * p[2].x + pow(tt, 3) * p[3].x;
		y = pow(1 - tt, 3) * p[0].y + 3 * pow(1 - tt, 2) * tt * p[1].y + 3 * (1 - tt) * pow(tt, 2) * p[2].y + pow(tt, 3) * p[3].y;
		glVertex2f(x, y);
	}
	glEnd();
	glLineWidth(lineWidth);
}

void drawCurrentPoint(double t) {
	Point currentPoint = linearBezier(r[0], r[1], t);
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex2f(currentPoint.x, currentPoint.y);
	glEnd();
}

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	drawControlPoints();

	double t = (1.0 * iFrame / numFrames) * (tFinal - tInitial);

	getIntermediatePoints(t);

	drawGreenLines();
	drawBlueLines();
	drawCurve(t);
	drawCurrentPoint(t);

	glutSwapBuffers();

	iFrame = (iFrame + 1) % numFrames;
	glutPostRedisplay();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case 'r':
		iFrame = 0;
		glutPostRedisplay();
		break;
	default: break;
	}
}

void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		p[iPoint].x = x;
		p[iPoint].y = screenH - y;
		iPoint = (iPoint + 1) % numPoints;
	}
	glutPostRedisplay();
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
	glutMouseFunc(myMouse);

	myInit();
	glutMainLoop();
}