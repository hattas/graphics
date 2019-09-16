#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

int x = 0;
int y = 0;

void myInit(void) {
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	glPointSize(4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 400, 0.0, 400);
}

void myDisplay(void) {
	// do nothing
	// draw lines and dots in keyboard callback
	// so we don't have to remember each dot/line
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case 'd':
		glBegin(GL_POINTS);
		glVertex2i(mouseX, 400-mouseY);
		glEnd();
		break;
	case 'l':
		glBegin(GL_LINE_STRIP);
		glVertex2i(x, 400-y);
		glVertex2i(mouseX, 400-mouseY);
		glEnd();
		break;
	default: break;
	}
	x = mouseX;
	y = mouseY;
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("HW 2 Dots and Lines");

	// register the callback functions
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);

	myInit();
	glutMainLoop();
}