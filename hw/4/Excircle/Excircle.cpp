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
const int screenW = 600, screenH = 600;
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
	glPointSize(5);
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

void drawBisectors() {
	glColor3d(1, 0.7, 0.7);
	for (int i = 0; i < numPoints; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex2d(midPoints[i].x, midPoints[i].y);
		glVertex2d(bisectPoints[i].x, bisectPoints[i].y);
		glEnd();
	}
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
	drawBisectors();
	drawCenter();
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

Point intersection(Point p1, double m1, Point p2, double m2) {
	double num, den, x, y;
	num = m1 * p1.x - m2 * p2.x - p1.y + p2.y;
	den = m1 - m2;
	x = num / den;
	y = m1 * (x - p1.x) + p1.y;
	Point p{ x, y };
	return p;
}

double distance(Point p1, Point p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void calculateBisectors() {
	// find intersection between each midpoint and other 2 pairs of lines
	// choose smaller line to show bisector
	int i, j, k;
	Point p1, p2;
	double d1, d2;
	for (i = 0; i < numPoints; i++) {
		j = (i + 1) % numPoints;
		k = (i + 2) % numPoints;
		p1 = intersection(midPoints[i], perpendicularSlopes[i], points[j], slopes[j]);
		p2 = intersection(midPoints[i], perpendicularSlopes[i], points[k], slopes[k]);
		d1 = distance(midPoints[i], p1);
		d2 = distance(midPoints[i], p2);
		if (d1 < d2)
			bisectPoints[i] = p1;
		else
			bisectPoints[i] = p2;
	}

	return;
}

void calculateCenter() {
	center = intersection(midPoints[0], perpendicularSlopes[0], midPoints[1], perpendicularSlopes[1]);
	radius = distance(center, points[0]);
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
	glutInitWindowPosition(500, 300);
	glutCreateWindow("HW 4 Excircle");

	glutDisplayFunc(display);
	glutMouseFunc(myMouse);

	init();
	glutMainLoop();
	return 0;
}
