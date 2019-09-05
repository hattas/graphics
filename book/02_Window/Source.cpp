#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// set up coordinate system, point size, background color, drawing color
void myInit(void) {
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 400, 0.0, 220);
}

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
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
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 220);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("big dipper");

	// register the callback functions
	glutDisplayFunc(myDisplay);
	/*glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);*/

	myInit();
	glutMainLoop();
}