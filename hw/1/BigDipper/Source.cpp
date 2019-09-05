#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

#define DIPPER 1
#define SMILE 2
#define Z 3
#define DIAMOND 4

GLfloat pointSize = 7.0;
int constellation = DIPPER; // current constellation

// set up coordinate system, point size, background color, drawing color
void myInit(void) {
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	glPointSize(pointSize);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 400, 0.0, 400);
}

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	switch (constellation) {
	case DIPPER:
		glBegin(GL_POINTS);
		glVertex2i(289, 190);
		glVertex2i(320, 128);
		glVertex2i(239, 67);
		glVertex2i(194, 101);
		glVertex2i(129, 83);
		glVertex2i(75, 73);
		glVertex2i(74, 74);
		glVertex2i(20, 10);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex2i(194, 101);
		glVertex2i(289, 190);
		glVertex2i(320, 128);
		glVertex2i(239, 67);
		glVertex2i(194, 101);
		glVertex2i(129, 83);
		glVertex2i(75, 73);
		glVertex2i(74, 74);
		glVertex2i(20, 10);
		glEnd();
		break;
	case SMILE:
		glBegin(GL_POINTS);
		glVertex2i(100, 300);
		glVertex2i(300, 300);
		glVertex2i(50, 120);
		glVertex2i(50, 120);
		glVertex2i(100, 100);
		glVertex2i(200, 90);
		glVertex2i(300, 100);
		glVertex2i(350, 120);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex2i(50, 120);
		glVertex2i(50, 120);
		glVertex2i(100, 100);
		glVertex2i(200, 90);
		glVertex2i(300, 100);
		glVertex2i(350, 120);
		glEnd();
		break;
	case Z:
		glBegin(GL_LINE_STRIP);
		glVertex2i(50, 350);
		glVertex2i(350, 350);
		glVertex2i(50, 50);
		glVertex2i(350, 50);
		glEnd();

		glBegin(GL_POINTS);
		glVertex2i(50, 350);
		glVertex2i(350, 350);
		glVertex2i(50, 50);
		glVertex2i(350, 50);
		glEnd();
		break;
	case DIAMOND:
		glBegin(GL_LINE_LOOP);
		glVertex2i(200, 350);
		glVertex2i(350, 200);
		glVertex2i(200, 50);
		glVertex2i(50, 200);
		glEnd();

		glBegin(GL_POINTS);
		glVertex2i(200, 350);
		glVertex2i(350, 200);
		glVertex2i(200, 50);
		glVertex2i(50, 200);
		glEnd();
		break;
	default:
		break;
	}
	glFlush();
}

void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON)
			pointSize--;
		else if (button == GLUT_RIGHT_BUTTON)
			pointSize++;
		glPointSize(pointSize);
		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case 'r': glColor3f(1, 0, 0); break;
	case 'g': glColor3f(0, 1, 0); break;
	case 'b': glColor3f(0, 0, 1); break;
	case 'w': glColor3f(1, 1, 1); break;
	case 'c': glColor3f(0, 1, 1); break;
	case 'm': glColor3f(1, 0, 1); break;
	case 'y': glColor3f(1, 1, 0); break;
	default: break;
	}
	glutPostRedisplay();
}

void processMenuEvents(int option) {
	switch (option) {
	case DIPPER: constellation = DIPPER; break;
	case SMILE: constellation = SMILE; break;
	case Z: constellation = Z; break;
	case DIAMOND: constellation = DIAMOND; break;
	default: break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("HW 1 Big Dipper");

	// register the callback functions
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	// setup menu
	glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Big Dipper", DIPPER);
	glutAddMenuEntry("Smile", SMILE);
	glutAddMenuEntry("Z", Z);
	glutAddMenuEntry("Diamond", DIAMOND);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	myInit();
	glutMainLoop();
}