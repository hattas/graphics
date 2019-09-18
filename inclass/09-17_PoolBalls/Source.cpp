#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>

// colors from https://flatuicolors.com/palette/au
#define RED 235, 77, 75
#define BLACK 50, 50, 50
#define WHITE 255, 255, 255
#define CUE_WHITE 255, 255, 237
#define GREEN 104, 207, 54
#define BROWN 240, 147, 43

// constants
const int ballRadius = 20;
const int screenW = 600;
const int screenH = 400;
const GLfloat maxForce = 10;
const GLfloat minForce = 0;
const GLfloat maxAngle = 90;
const GLfloat minAngle = -90;
const GLfloat forceIncrement = (maxForce - minForce) / 100.0;
const GLfloat angleIncrement = (maxAngle - minAngle) / 180.0;
const GLfloat guideLength = 100;
const GLfloat PI = (GLfloat)3.1415926535897932384626433832795;
const GLfloat veloThreshold = maxForce / 100;

// globals
GLfloat force = maxForce / 2;
GLfloat launchAngle = 0;
GLfloat launchAngleRadians = 0;
GLfloat currentAngle = 0;

GLfloat cueX = ballRadius;
GLfloat cueY = screenH / 2;
GLfloat eightX = screenW / 2;
GLfloat eightY = screenH / 2;
GLfloat cueVX = 0;
GLfloat cueVY = 0;
GLfloat eightVX = 0;
GLfloat eightVY = 0;
GLfloat cueDirX;
GLfloat cueDirY;
GLfloat friction = 0.2;
boolean guideVisible = true;

void color255(int r, int g, int b) {
	glColor3f((GLfloat)(r / 255.0), (GLfloat)(g / 255.0), (GLfloat)(b / 255.0));
}

void myInit(void) {
	glClearColor(0, 0, 0, 0);
	color255(WHITE);
	glPointSize(4);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screenW, 0.0, screenH);
}

void drawForceBar(void) {
	color255(WHITE);
	int left = screenW / 16;
	int right = screenW / 8;
	int bot = screenH / 8;
	int top = screenH / 8 * 7;
	glRecti(left, bot, right, top);
	color255(RED);
	double percentForce = force / maxForce;
	glRecti(left, bot, right, (screenH / 8 * 7 - bot) * force / maxForce + bot);
	glRecti(left, bot, right, (GLint)((top - bot) * percentForce + bot));
}

void drawPoolTable(void) {
	color255(GREEN);
	glRecti(0, 0, screenW, screenH);
}

void drawCircle(GLfloat x, GLfloat y, int radius) {
	glBegin(GL_POLYGON);
	for (GLfloat i = 0; i < 2 * PI; i += PI / 40)
		glVertex2f(x + (GLfloat)cos(i) * radius, y + (GLfloat)sin(i) * radius);
	glEnd();
}

void drawCueBall(void) {
	color255(CUE_WHITE);
	drawCircle(cueX, cueY, ballRadius);
}

void drawEightBall(void) {
	color255(BLACK);
	drawCircle(eightX, eightY, ballRadius);
}

void drawGuide(void) {
	color255(WHITE);
	glBegin(GL_LINE_STRIP);
	glVertex2f(cueX, cueY);
	glVertex2f(cueX + guideLength * cos(launchAngleRadians), cueY + guideLength * sin(launchAngleRadians));
	glEnd();
}

void myDisplay(void) {
	std::cout << cueVX << ' ' << cueVY << '\n';
	// adjust ball positions
	cueX += cueVX;
	cueY += cueVY;

	// check for collisions
	if ((cueX - ballRadius) < 0) {
		cueX = ballRadius;
		cueVX = -cueVX;
	}
	if ((cueX + ballRadius) > screenW) {
		cueX = screenW - ballRadius;
		cueVX = -cueVX;
	}
	if ((cueY - ballRadius) < 0) {
		cueY = ballRadius;
		cueVY = -cueVY;
	}
	if ((cueY + ballRadius) > screenH) {
		cueY = screenH - ballRadius;
		cueVY = -cueVY;
	}

	// add friction
	if (cueVX > 0)
		cueDirX = 1;
	else
		cueDirX = -1;
	if (cueVY > 0)
		cueDirY = 1;
	else
		cueDirY = -1;
	if (cueVX != 0) {
		currentAngle = atan(cueVY / cueVX);
		cueVX -= cueDirX * friction * abs(cos(currentAngle));
		cueVY -= cueDirY * friction * abs(sin(currentAngle));
	}
	else {
		cueVY -= cueDirY * friction;
	}

	// check for small velo and set to 0
	if (abs(sqrt(cueVX * cueVX + cueVY * cueVY) < veloThreshold)) {
		cueVX = 0;
		cueVY = 0;
	}


	glViewport(0, 0, screenW, screenH);
	drawForceBar();

	GLsizei tableH = 3 * screenH / 4;
	GLsizei tableW = tableH / 2 * 3;
	glViewport(3 * screenW / 16, screenH / 8, tableW, tableH);
	drawPoolTable();
	if (guideVisible)
		drawGuide();
	drawCueBall();
	drawEightBall();

	glutSwapBuffers();

	// if movement, update
	glutPostRedisplay(); // loop constantly
}

void mySpecialKeyboard(int theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case GLUT_KEY_UP: // force up
		force += forceIncrement;
		if (force > maxForce)
			force = maxForce;
		break;
	case GLUT_KEY_DOWN: // force down
		force -= forceIncrement;
		if (force < minForce)
			force = minForce;
		break;
	case GLUT_KEY_LEFT: // angle up
		launchAngle += angleIncrement;
		if (launchAngle > maxAngle)
			launchAngle = maxAngle;
		break;
	case GLUT_KEY_RIGHT: // angle down
		launchAngle -= angleIncrement;
		if (launchAngle < minAngle)
			launchAngle = minAngle;
		break;
	}
	launchAngleRadians = (launchAngle * PI) / 180;
	std::cout << force << ' ' << launchAngle << '\n';
	glutPostRedisplay();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case ' ':
		cueVX = force * cos(launchAngleRadians);
		cueVY = force * sin(launchAngleRadians);
		guideVisible = false;
		break;
	default: break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Pool Balls Simulation");

	// register the callback functions
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(mySpecialKeyboard);
	glutKeyboardFunc(myKeyboard);

	myInit();
	glutMainLoop();
}