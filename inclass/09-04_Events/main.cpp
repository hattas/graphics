#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

int mx = -1;
int my = -1;
bool first = true;
const int screenwidth = 640;
const int screenheight = 480;

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
	gluOrtho2D(0.0, screenwidth, 0.0, screenheight);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		if (first)
			first = false;
		else {
			glRecti(mx, my, x, screenheight - y);
			glFlush();
		}
		mx = x;
		my = screenheight - y;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenwidth, screenheight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("My First Window");
	glutDisplayFunc(display);
	glutMouseFunc(myMouse);
	init();
	glutMainLoop();
	return 0;
}
