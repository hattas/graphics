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
const int numPoints = 12;
const double tInitial = 0;
const double tFinal = 1;
const int numFrames = 200;
const double stepSize = (tFinal - tInitial) / numFrames;

char filenameM[] = "m.dat";
char filenameU[] = "u.dat";
int iFrame = 0; // frame number of animation
int animationDirection = 1;
int loopAnimation = 1;
int animate = 1;

// globals
Point mPoints[numPoints];
Point uPoints[numPoints];

void loadPolyLineFile(char* fileName, Point* points) {
	fstream inStream;
	inStream.open(fileName, ios::in);
	if (inStream.fail())
		return;
	GLint numpolys, numLines, x, y;
	inStream >> numpolys;
	for (int j = 0; j < numpolys; j++) {
		inStream >> numLines;
		for (int i = 0; i < numLines; i++) {
			inStream >> x >> y;
			points[i] = { (double)x, (double)y };
		}
	}
}

void init(void) {
	// open gl init
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(1);
	glLineWidth(2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenW, 0, screenH);

	// load point files for m and u
	loadPolyLineFile(filenameM, mPoints);
	loadPolyLineFile(filenameU, uPoints);
}

void drawPolyLineFile(char* fileName) {
	fstream inStream;
	inStream.open(fileName, ios::in);
	if (inStream.fail())
		return;
	glClear(GL_COLOR_BUFFER_BIT);
	GLint numpolys, numLines, x, y;
	inStream >> numpolys;
	for (int j = 0; j < numpolys; j++) {
		inStream >> numLines;
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < numLines; i++) {
			inStream >> x >> y;
			glVertex2i(x, y);
		}
		glEnd();
	}
	glFlush();
	inStream.close();
}

Point lerp(Point a, Point b, double t) {
	Point c = { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
	return c;
}

void drawTween(double t) {
	Point lerped;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numPoints; i++) {
		lerped = lerp(mPoints[i], uPoints[i], t);
		glVertex2f((GLfloat)lerped.x, (GLfloat)lerped.y);
	}
	glEnd();
}

void getNextFrameIndex() {
	iFrame += animationDirection;
	if (iFrame < 0) {
		iFrame = 0;
		animationDirection = 1;
	}
	else if (iFrame >= numFrames) {
		iFrame = numFrames - 1;
		animationDirection = -1;
	}
}

void setColor(double t) {
	glColor3f(0, 0, 0);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	double t = (1.0 * iFrame / numFrames) * (tFinal - tInitial);

	setColor(t);
	drawTween(t);

	glutSwapBuffers();

	getNextFrameIndex();

	glutPostRedisplay();
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HW 4 MU Tween");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
