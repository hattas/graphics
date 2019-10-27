#include "sdl_arch.h"
#include "camera.h"
#include <conio.h>

Scene scn;		// the scene object
Camera camera;	// the camera object

float screenWidth = 640, screenHeight = 480;

float R = screenWidth / screenHeight;  // Global Aspect Ratio

Mesh* myMesh = new Mesh();


void myKeyboard(unsigned char key, int x, int y) {
	float moveAmount = .01;
	switch (key) {
	case 'F':
		camera.slide(0, 0, moveAmount);
		break;
	case 'f':
		camera.slide(0, 0, -moveAmount);
		break;
	case 'W':
	case 'w':
		camera.slide(0, moveAmount, 0);
		break;
	case 'Z':
		camera.slide(0, moveAmount, 0);
		break;
	case 'z':
		camera.slide(0, -moveAmount, 0);
		break;
	case 'S':
		camera.slide(-moveAmount, 0, 0);
		break;
	case 's':
		camera.slide(moveAmount, 0, 0);
		break;
	case 'A':
	case 'a':
		camera.slide(-moveAmount, 0, 0);
		break;
	case 'P':
		//case 80:
		camera.pitch(-1);
		break;
	case 'p':
		camera.pitch(1);
		break;
	case 'R':
		camera.roll(-1);
		break;
	case 'r':
		camera.roll(1);
		break;
	case 'Y':
		camera.yaw(-1);
		break;
	case 'y':
		camera.yaw(1);
		break;
	case '1':
		camera.set(0, 0, 40, 0, 0, 0, 0, 1, 0);
		break;
	case '2':
		camera.set(-40, 0, 0, 0, 0, 0, 0, 1, 0);
		break;
	case '3':
		camera.set(-70, 0, 0, 0, 0, 0, 0, 1, 0);
		break;
	case '4':
		camera.set(25, -50, -25, 25, 0, -25, 0, 1, 0);
		break;
	default:
		;

	}

	glutPostRedisplay();
}

void myDisplay() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scn.drawSceneOpenGL();

	static float r = 1.0;
	static float g = 0.0;
	static float b = 0.0;
	if (b > 1)
		b = b - 1;

	// call to makeSmoothTube
	glPushMatrix();
	glRotated(90.0, -1.0, 0.0, 0.0);
	glTranslatef(26.0, 27.0, -2.0);
	glScalef(0.75, 0.75, 0.75);

	myMesh->makeSmoothTube();

	myMesh->mtrl.ambient.set(1 - b * b, b, 1.0 - b);
	b = b + 0.025;

	myMesh->drawOpenGL();

	glPopMatrix();

	glutSwapBuffers();	// added 8/29
}


void setViewport(int left, int right, int bottom, int top) {
	glViewport(left, bottom, right - left, top - bottom);
}

void myReshape(GLsizei W, GLsizei H) {
	if (R > W / H)
		setViewport(0, W, 0, W / R);
	else
		setViewport(0, H * R, 0, H);
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	glutInitWindowSize(640, 480);

	glutInitWindowPosition(100, 150);
	glutCreateWindow("HW 5: SDL");

	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glViewport(0, 0, 640, 480);

	camera.set(2, 1, 2, 0.6, .4, 0.6, 0, 1, 0);
	camera.setShape(35.0f, 64.0f / 48.0f, 0.5f, 140.0f);

	scn.read("myScene1.dat");
	glEnable(GL_LIGHTING);
	scn.makeLightsOpenGL();
	double winHt = 1.0;
	myDisplay();
	glutMainLoop();
}
