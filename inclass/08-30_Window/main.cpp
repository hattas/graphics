#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void init(void)
{
	// setup coordinate system
	// setup colors -> background and dots
	// setup matrix mode
	// 2 matrix modes: projection or model view
	// we use projection usually
	glColor3f(0.0f, 1.0f, 1.0f);
	glPointSize(14.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void display(void)
{
	// clear the frame buffer
	// draw
	// flush the frame buffer
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glVertex2i(100, 100);
	glVertex2i(200, 100);
	glVertex2i(300, 100);
	glVertex2i(400, 100);
	glEnd();
	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("My First Window");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
