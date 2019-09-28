#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
using namespace std;

struct Point {
	double x, y;
};

// constants
const int screenW = 400, screenH = 400;
const int numPoints = 3;

// globals
Point points[numPoints];
Point midPoints[numPoints];
int numPointsSelected = 0;

void init(void) {
	// open gl init
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(4);
	glLineWidth(2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenW, 0, screenH);
}


Point lerp(Point a, Point b, double t) {
	Point c = { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
	return c;
}

void drawPoints() {
	glColor3d(0.7, 0.7, 0.7);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numPointsSelected; i++) {
		glVertex2d(points[i].x, points[i].y);
	}
	glEnd();
	glColor3d(0, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < numPointsSelected; i++) {
		glVertex2d(points[i].x, points[i].y);
	}
	glEnd();
}

void drawMidPoints() {
	glColor3d(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < numPoints; i++) {
		glVertex2d(midPoints[i].x, midPoints[i].y);
	}
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	drawPoints();
	if (numPointsSelected < numPoints) {
		glutSwapBuffers();
		return;
	}
	else {
		drawMidPoints();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void calculateMidpoints() {
	int i, j;
	for (i = 0; i < numPoints; i++) {
		j = (i + 1) % numPoints;
		midPoints[i] = lerp(points[i], points[j], 0.5);
	}
}

int checkIfPointsAreCollinear() {
	double slopes[numPoints - 1];
	for (int i = 0; i < numPoints - 1; i++) {
		if (points[i + 1].x - points[i].x == 0)
			slopes[i] = 0;
		else
			slopes[i] = (points[i + 1].y - points[i].y) / (points[i + 1].x - points[i].x);
	}
	return slopes[1] == slopes[0];
}

void myMouse(int button, int state, int x, int y) {
	// left button clicked
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		// if we didnt select all the points yet
		if (numPointsSelected < numPoints) {
			points[numPointsSelected] = { (double)x, screenH - (double)y };
			numPointsSelected++;
			// if user drew all points, check if they are collinear
			// if they are, reset
			if (numPointsSelected == numPoints) {
				if (checkIfPointsAreCollinear())
					numPointsSelected = 0;
				else
					calculateMidpoints();
			}
			glutPostRedisplay();
		}
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HW 4 Excircle");

	glutDisplayFunc(display);
	glutMouseFunc(myMouse);

	init();
	glutMainLoop();
	return 0;
}
