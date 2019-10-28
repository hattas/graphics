#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;

const int SCREEN_W = 400, SCREEN_H = 400;

void init(void) {
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 1);
	glPointSize(6);
	glLineWidth(2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double ortho = 2.5;
	gluOrtho2D(-ortho, ortho, -ortho, ortho);
}

void glVertexAdjusted(float x, float y) {
	glVertex2f(x + 0.5, y + .6882);
}

void starMotif() {
	glBegin(GL_LINE_LOOP);
	glVertexAdjusted(-.4975, 1.528);
	glVertexAdjusted(0, 0);
	glVertexAdjusted(-.25, 0);
	glVertexAdjusted(-.5, .8);
	glVertexAdjusted(-1.06, -.86);
	glVertexAdjusted(-1.24, -.73);
	glEnd();
}

void rotate2D(double angle) {
	glMatrixMode(GL_MODELVIEW);
	glRotated(angle, 0, 0, 1);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 5; i++) {
		starMotif();
		rotate2D(72);
	}

	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("HW 5: Star with Rotation");

	glutDisplayFunc(display);

	init();
	glutMainLoop();
	return 0;
}
