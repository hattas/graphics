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

std::ostream& operator<< (std::ostream& out, Point const& p) {
	out << "(" << p.x << ", " << p.y << ")";
	return out;
}

// constants
const int screenW = 400, screenH = 400;
const int numPoints = 3;
const double PI = 3.1415926535897932384626433832795;

// globals
Point points[numPoints];
Point midPoints[numPoints];
double slopes[numPoints];
double perpendicularSlopes[numPoints];
Point bisectPoints[numPoints];
int numPointsSelected = 0;
Point center;
double radius;

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

void drawCenter() {
	glColor3d(0, 0, 1);
	glBegin(GL_POINTS);
	glVertex2d(center.x, center.y);
	glEnd();
}

void drawCircle(double x, double y, double radius) {
	glColor3d(0, 1, 0);
	glBegin(GL_LINE_LOOP);
	for (double i = 0; i < 2 * PI; i += PI / 40)
		glVertex2f(x + cos(i) * radius, y + sin(i) * radius);
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	if (numPointsSelected < numPoints) {
		drawPoints();
		glutSwapBuffers();
		return;
	}

	
	drawCenter();
	drawCircle(center.x, center.y, radius);
	drawPoints();
	drawMidPoints();


	glutSwapBuffers();
	glutPostRedisplay();
}

void calculateMidpoints() {
	int i, j;
	for (i = 0; i < numPoints; i++) {
		j = (i + 1) % numPoints;
		midPoints[i] = lerp(points[i], points[j], 0.5);
		cout << "midPoints[" << i << "] = " << midPoints[i] << "\n";
	}
}

void calculateSlopes() {
	int i, j;
	for (i = 0; i < numPoints; i++) {
		j = (i + 1) % numPoints;
		if (points[j].x - points[i].x == 0)
			slopes[i] = 0;
		else
			slopes[i] = (points[j].y - points[i].y) / (points[j].x - points[i].x);

		perpendicularSlopes[i] = -1 / slopes[i];
		cout << "slopes[" << i << "] = " << slopes[i] << " " << "perpendicularSlopes[" << i << "] = " << perpendicularSlopes[i] << "\n";
	}
}

void calculateBisectors() {
	return;
}

Point intersection(Point p1, double m1, Point p2, double m2) {
	double num, den, x, y;
	num = m1 * p1.x - m2 * p2.x - p1.y + p2.y;
	den = m1 - m2;
	x = num / den;
	y = m1 * (x - p1.x) + p1.y;
	Point p{ x, y };
	return p;
}

void calculateCenter() {
	center = intersection(midPoints[0], perpendicularSlopes[0], midPoints[1], perpendicularSlopes[1]);
	radius = sqrt(pow(center.x - points[0].x, 2) + pow(center.y - points[0].y, 2));
	cout << "center = " << center << "\n";
	cout << "radius = " << radius << "\n";
}

void calculateStuff() {
	calculateMidpoints();
	calculateBisectors();
	calculateCenter();
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
				calculateSlopes();
				if (slopes[0] == slopes[1])
					numPointsSelected = 0;
				else
					calculateStuff();
			}
			glutPostRedisplay();
		}
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(500, 500);
	glutCreateWindow("HW 4 Excircle");

	glutDisplayFunc(display);
	glutMouseFunc(myMouse);

	init();
	glutMainLoop();
	return 0;
}
