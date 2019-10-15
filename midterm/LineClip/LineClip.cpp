#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;

struct Point {
	double x, y;
};

struct Rect {
	double left, right, top, bottom;
};

const int SCREEN_W = 600, SCREEN_H = 600;
const double RECT_LEFT = SCREEN_W / 3;
const double RECT_RIGHT = 2 * SCREEN_W / 3;
const double RECT_TOP = 2 * SCREEN_H / 3;
const double RECT_BOT = SCREEN_H / 3;
const char LEFT = 1 << 3;
const char ABOVE = 1 << 2;
const char RIGHT = 1 << 1;
const char BELOW = 1 << 0;
const Rect window = { RECT_LEFT , RECT_RIGHT, RECT_TOP, RECT_BOT };

int numPoints = 0;
Point A, B;
Point startA, startB; // record starting points because the algorithm will modify points A and B
bool lineVisible = false;

void drawWindow() {
	glBegin(GL_LINE_LOOP);
	glVertex2d(window.left, window.bottom);
	glVertex2d(window.right, window.bottom);
	glVertex2d(window.right, window.top);
	glVertex2d(window.left, window.top);
	glEnd();
}

void drawLine(Point p1, Point p2) {
	glBegin(GL_LINE_STRIP);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glEnd();
}

void drawGrid() {
	drawLine({ 0, window.bottom }, { SCREEN_W, window.bottom });
	drawLine({ 0, window.top }, { SCREEN_W, window.top });
	drawLine({ window.left, 0 }, { window.left, SCREEN_H });
	drawLine({ window.right, 0 }, { window.right, SCREEN_H });
}

void drawPoint(Point p) {
	glBegin(GL_POINTS);
	glVertex2d(p.x, p.y);
	glEnd();
}

int getCode(Point p) {
	// page 106
	int code = 0;
	if (p.x < RECT_LEFT)
		code |= LEFT;
	if (p.y > RECT_TOP)
		code |= ABOVE;
	if (p.x > RECT_RIGHT)
		code |= RIGHT;
	if (p.y < RECT_BOT)
		code |= BELOW;
	return code;
}

void init(void) {
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(6);
	glLineWidth(1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_W, 0, SCREEN_H);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3d(0.8, 0.8, 0.8);
	drawGrid();
	glColor3d(0, 0, 0);
	drawWindow();

	if (lineVisible && numPoints == 2) {
		glColor3d(0.8, 0.8, 0.8);
		drawLine(startA, startB);
		glColor3d(1, 0, 0);
		drawLine(A, B);
		drawPoint(A);
		drawPoint(B);
		glColor3d(0, 0, 0);
	}

	if (numPoints == 1) {
		drawPoint(startA);
	}
	else if (numPoints == 2) {
		drawPoint(startA);
		drawPoint(startB);
	}

	glFlush();
}

bool calculateEndpoints() {
	double dely, delx;
	int codeA, codeB;
	do {
		cout << "A " << A.x << ", " << A.y << endl;
		cout << "B " << B.x << ", " << B.y << endl;

		// form the code words for p1 and p2
		codeA = getCode(A);
		codeB = getCode(B);

		// trivial accept
		if (codeA == 0 && codeB == 0)
			return true;

		// trivial reject
		// code words have a T in the same element
		// i.e. both are above the window
		if (codeA & codeB) {
			return false;
		}

		// A is outside
		if (codeA) {
			delx = B.x - A.x;
			dely = B.y - A.y;

			// chop against left edge, update A
			if (codeA & LEFT) {
				A.y += (window.left - A.x) * dely / delx;
				A.x = window.left;
			}
			else if (codeA & RIGHT) {
				A.y += (window.right - A.x) * dely / delx;
				A.x = window.right;
			}
			else if (codeA & BELOW) {
				A.x += (window.bottom - A.y) * delx / dely;
				A.y = window.bottom;
			}
			else if (codeA & ABOVE) {
				A.x += (window.top - A.y) * delx / dely;
				A.y = window.top;
			}
		}
		// B is outside, just swap them to reuse the code for A being outside
		else if (codeB) {
			Point temp;
			temp = A;
			A = B;
			B = temp;
		}
	} while (1);
}

void myMouse(int button, int state, int x, int y) {
	if (!(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON))
		return;

	if (numPoints == 0 || numPoints == 2) {
		A = Point{ (double)x, (double)SCREEN_H - y };
		startA = A;
		numPoints = 1;
		lineVisible = false;
	}
	else {
		B = Point{ (double)x, (double)SCREEN_H - y };
		startB = B;
		lineVisible = calculateEndpoints();
		numPoints = 2;
	}
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case 'r':
		numPoints = 0;
		glutPostRedisplay();
		break;
	default: break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Midterm: Line Clipping");

	glutDisplayFunc(display);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	init();
	glutMainLoop();
	return 0;
}
