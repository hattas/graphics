#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

struct Point {
	GLfloat x, y;
};

// constants
const int numPoints = 4;
const int screenW = 400;
const int screenH = 400;

// globals
Point points[numPoints];

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
	for (int i = 0; i < numPoints; i++) {
		glVertex2f(points[i].x, points[i].y);
	}
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
	glClear(GL_COLOR_BUFFER_BIT);
	
	drawFourPoints();
	
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("HW 3 Bezier Animation");

	// register the callback functions
	glutDisplayFunc(myDisplay);

	myInit();
	glutMainLoop();
}