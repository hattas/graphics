#include <windows.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>
#include <math.h>

// demo program to draw a rosette based on a 5-gon 

const int NUM_POINTS = 5;
const int NUM_LEVELS = 3;
const int SCREEN_W = 400, SCREEN_H = 400;
const double RADIUS_OUTER = SCREEN_W / 2.5;
const double PI = 3.1415926536;
const double PHI = 1.61803398875;


class GLintPoint {
public:
	GLint x, y;
};


// point 2 class
class Point2 {
public:
	float x, y;
	void set(float dx, float dy) { x = dx; y = dy; }
	void set(Point2& p) { x = p.x; y = p.y; }
	Point2(float xx, float yy) { x = xx; y = yy; }
	Point2() { x = y = 0; }
};

Point2 currPos;
Point2 CP;

void moveTo(Point2 p) {
	CP.set(p);
}

void lineTo(Point2 p) {
	glBegin(GL_LINES);
	glVertex2f(CP.x, CP.y);
	glVertex2f(p.x, p.y);
	glEnd();
	glFlush();
	CP.set(p);
}

void myInit(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 1, 0);
	glColor3d(6 / 255.0, 162 / 255.0, 232 / 255.0);
	glLineWidth(1.5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-SCREEN_W / 2, SCREEN_W / 2, -SCREEN_H / 2, SCREEN_H / 2);
}

void rosette(int N, float radius, int flipped) {
	Point2* pointlist = new Point2[N];
	GLfloat theta = (2.0f * PI) / N;
	double offset = 0;

	if (flipped) {
		offset = PI;
	}


	for (int c = 0; c < N; c++) {
		pointlist[c].set(radius * sin(offset + theta * c), radius * cos(offset + theta * c));
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			moveTo(pointlist[i]);
			lineTo(pointlist[j]);
		}
	}
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	double radius = RADIUS_OUTER;
	int flipped = false;
	for (int i = 0; i < NUM_LEVELS; i++) {
		rosette(NUM_POINTS, radius, flipped);
		flipped = !flipped;
		radius /= (PHI * PHI);
	}
	glFlush();
}


void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutCreateWindow("Rosette");
	glutDisplayFunc(render);// register the callback for the display function
	myInit();
	glutMainLoop();
}
