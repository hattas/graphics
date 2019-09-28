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

struct Color {
	double r, g, b;
};

// constants
const int screenW = 400, screenH = 400;
const int numPoints = 12;
const double tInitial = 0;
const double tFinal = 1;
const int numFrames = 200;
const double stepSize = (tFinal - tInitial) / numFrames;
const Color blue{ 0, 0, 1 };
const Color yellow{ 1, 1, 0 };

char filenameM[] = "m.dat";
char filenameU[] = "u.dat";
int iFrame = 0; // frame number of animation
int animationDirection = 1; // 1 or -1 - 1 means animating from m to u
int loopAnimation = 1; // boolean to loop animation when we reach m or u
int animate = 1; // play or pause animation

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

Color lerp(Color a, Color b, double t) {
	Color c = { a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t };
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
		if (!loopAnimation)
			animate = 0;
	}
	else if (iFrame >= numFrames) {
		iFrame = numFrames - 1;
		animationDirection = -1;
		if (!loopAnimation)
			animate = 0;
	}
}

void setColor(double t) {
	Color lerpedColor = lerp(blue, yellow, t);
	glColor3f(lerpedColor.r, lerpedColor.g, lerpedColor.b);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	double t = (1.0 * iFrame / numFrames) * (tFinal - tInitial);

	setColor(t);
	drawTween(t);

	glutSwapBuffers();

	if (animate) {
		getNextFrameIndex();
	}

	glutPostRedisplay();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch(theKey) {
	case 'm':
		animationDirection = 1;
		iFrame = 0;
		loopAnimation = 0;
		glutPostRedisplay();
		break;
	case 'u':
		animationDirection = -1;
		iFrame = numFrames - 1;
		loopAnimation = 0;
		glutPostRedisplay();
		break;
	case 'a':
		animate = 1;
		loopAnimation = 1;
		glutPostRedisplay();
		break;
	case 's':
		animate = 0;
		break;
	case 'e':
		exit(0);
		break;
	default: break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HW 4 MU Tween");
	glutDisplayFunc(display);
	glutKeyboardFunc(myKeyboard);
	init();
	glutMainLoop();
	return 0;
}
