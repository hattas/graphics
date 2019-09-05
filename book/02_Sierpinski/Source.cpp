#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

struct GLintPoint {
	GLint x, y;
};

int random(int m) {
	return rand() % m;
}

void drawDot(GLint x, GLint y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void myInit(void) {
	glClearColor(1, 1, 1, 0);
	glColor3f(0, 0, 0);
	glPointSize(2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800, 0.0, 600);
}

void sierpinksi_render() {
	glClear(GL_COLOR_BUFFER_BIT);
	GLintPoint T[3] = { {10,10}, {600,10}, {300,600} };

	int index = rand() % 3;
	GLintPoint point = T[index];
	drawDot(point.x, point.y);
	for (int i = 0; i < 55000; i++) {
		index = random(3);
		point.x = (point.x + T[index].x) / 2;
		point.y = (point.y + T[index].y) / 2;
		drawDot(point.x, point.y);
	}
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Sierpinski");

	// register the callback functions
	glutDisplayFunc(sierpinksi_render);
	/*glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);*/

	myInit();
	glutMainLoop();
}