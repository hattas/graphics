#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;

const int SCREEN_W = 1200, SCREEN_H = 800;
const int VIEWPORT_SIZE = 400;
const double PI = 3.1415926535897932384626433832795;
const double CENTER_X = VIEWPORT_SIZE / 2, CENTER_Y = VIEWPORT_SIZE / 2;
const double RADIUS_OUTER = VIEWPORT_SIZE / 2.5;
const double RADIUS_INNER = RADIUS_OUTER / 2.5;
const int CIRCLE_SIDES = 10;
const double CIRCLE_INC = 2 * PI / CIRCLE_SIDES;

void drawCircle(double x, double y, double radius, char wireframe) {
	if (wireframe)
		glPolygonMode(GL_FRONT, GL_LINE);
	else
		glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * PI; i += CIRCLE_INC)
		glVertex2d((GLdouble)(x + cos(i) * radius), (GLdouble)(y + sin(i) * radius));
	glEnd();
}

void drawCircleTriangle(double x, double y, double radius) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(x, y);
	for (double i = 0; i <= 2 * PI + CIRCLE_INC; i += CIRCLE_INC)
		glVertex2d((GLdouble)(x + cos(i) * radius), (GLdouble)(y + sin(i) * radius));
	glEnd();
}

void drawAnnulusTriangle(double x, double y, double radiusInner, double radiusOuter, char wireframe) {

	double inc = 2 * PI / CIRCLE_SIDES;;
	for (double i = 0; i <= 2 * PI + CIRCLE_INC; i += CIRCLE_INC) {
		if (wireframe)
			glPolygonMode(GL_FRONT, GL_LINE);
		else
			glPolygonMode(GL_FRONT, GL_FILL);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2d((GLdouble)(x + cos(i) * radiusInner), (GLdouble)(y + sin(i) * radiusInner));
		glVertex2d((GLdouble)(x + cos(i) * radiusOuter), (GLdouble)(y + sin(i) * radiusOuter));
		glVertex2d((GLdouble)(x + cos(i + inc) * radiusOuter), (GLdouble)(y + sin(i + inc) * radiusOuter));
		glVertex2d((GLdouble)(x + cos(i + inc) * radiusInner), (GLdouble)(y + sin(i + inc) * radiusInner));
		glEnd();
	}
}

void a_drawCirclePolygon() {
	glColor3d(6 / 255.0, 162 / 255.0, 232 / 255.0);
	drawCircle(CENTER_X, CENTER_Y, RADIUS_OUTER, false);
}

void b_drawAnnulusPolygon() {
	glColor3d(6 / 255.0, 162 / 255.0, 232 / 255.0);
	drawCircle(CENTER_X, CENTER_Y, RADIUS_OUTER, false);
	glColor3d(1, 1, 1);
	drawCircle(CENTER_X, CENTER_Y, RADIUS_INNER, false);
}

void c_drawAnnulusWireframePolygon() {
	glColor3d(6 / 255.0, 162 / 255.0, 232 / 255.0);
	drawCircle(CENTER_X, CENTER_Y, RADIUS_OUTER, true);
	drawCircle(CENTER_X, CENTER_Y, RADIUS_INNER, true);
}

void a_drawCircleTriangleFan() {
	glColor3d(6 / 255.0, 162 / 255.0, 232 / 255.0);
	drawCircleTriangle(CENTER_X, CENTER_Y, RADIUS_OUTER);
}

void b_drawAnnulusTriangleFan() {
	glColor3d(6 / 255.0, 162 / 255.0, 232 / 255.0);
	drawAnnulusTriangle(CENTER_X, CENTER_Y, RADIUS_INNER, RADIUS_OUTER, false);
}

void c_drawAnnulusWireframeTriangleFan() {
	glColor3d(6 / 255.0, 162 / 255.0, 232 / 255.0);
	drawAnnulusTriangle(CENTER_X, CENTER_Y, RADIUS_INNER, RADIUS_OUTER, true);
}

void init(void) {
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(5);
	glLineWidth(2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, VIEWPORT_SIZE, 0, VIEWPORT_SIZE);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// part a - just draw the circle
	glViewport(0, VIEWPORT_SIZE, VIEWPORT_SIZE, VIEWPORT_SIZE);
	a_drawCirclePolygon();
	glViewport(0, 0, VIEWPORT_SIZE, VIEWPORT_SIZE);
	a_drawCircleTriangleFan();

	// part b - draw the annulus
	glViewport(VIEWPORT_SIZE, VIEWPORT_SIZE, VIEWPORT_SIZE, VIEWPORT_SIZE);
	b_drawAnnulusPolygon();
	glViewport(VIEWPORT_SIZE, 0, VIEWPORT_SIZE, VIEWPORT_SIZE);
	b_drawAnnulusTriangleFan();

	// part c - draw the wireframe
	glViewport(2 * VIEWPORT_SIZE, VIEWPORT_SIZE, VIEWPORT_SIZE, VIEWPORT_SIZE);
	c_drawAnnulusWireframePolygon();
	glViewport(2 * VIEWPORT_SIZE, 0, VIEWPORT_SIZE, VIEWPORT_SIZE);
	c_drawAnnulusWireframeTriangleFan();

	glutSwapBuffers();
}

void myMouse(int button, int state, int x, int y) {

}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutInitWindowPosition(500, 300);
	glutCreateWindow("Midterm: Circle");

	glutDisplayFunc(display);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	init();
	glutMainLoop();
	return 0;
}
