#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

struct House {
	GLint w, h;
	GLfloat r, g, b;
	boolean destroyed;
};

const int numHouses = 16;
const int screenWidth = 640;
const int screenHeight = 480;
const int dinoW = screenWidth / 8;
const int dinoH = screenHeight / 8;

GLint dinoX = 0;
GLint dinoY = 0;

House houses[numHouses];

void init(void) {
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	glPointSize(1);
	glLineWidth(1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);

	GLfloat r, g, b;
	GLint w, h;
	// get random params for houses
	for (int i = 0; i < numHouses; i++) {
		r = (GLfloat)(rand() % 255) / 255;
		g = (GLfloat)(rand() % 255) / 255;
		b = (GLfloat)(rand() % 255) / 255;
		w = rand() % (screenWidth / 3) + screenWidth / 3;
		h = rand() % (screenHeight / 3) + screenHeight / 3;
		houses[i] = { w, h, r, g, b, false };
	}
}

void setViewport(GLint left, GLint right, GLint bottom, GLint top) {
	glViewport(left, bottom, right - left, top - bottom);
}

void parameterizedHouse(GLint x, GLint y, GLint width, GLint height) {
	// x and y are the peak of the house (top)
	// draw house
	glBegin(GL_LINE_LOOP);
	glVertex2i(x, y);
	glVertex2i(x + width / 2, y - 3 * height / 8);
	glVertex2i(x + width / 2, y - height);
	glVertex2i(x - width / 2, y - height);
	glVertex2i(x - width / 2, y - 3 * height / 8);
	glEnd();
	// draw door
	glBegin(GL_LINE_LOOP);
	glVertex2i(x, y - height);
	glVertex2i(x, y - 5 * height / 8);
	glVertex2i(x - width / 5, y - 5 * height / 8);
	glVertex2i(x - width / 5, y - height);
	glEnd();
	glFlush();
}


void drawPolyLineFile(char* fileName) {
	fstream inStream;
	inStream.open(fileName, ios::in);
	if (inStream.fail())
		return;
	GLint numpolys, numLines, x, y;
	inStream >> numpolys;
	for (int j = 0; j < numpolys; j++) {
		inStream >> numLines;
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < numLines; i++) {
			inStream >> x >> y;
			glVertex2i(x, y);
		}
		glEnd();
	}
	inStream.close();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	// draw houses
	int row, col;
	for (int i = 0; i < numHouses; i++) {
		row = i / 4;
		col = i % 4;
		glViewport(row * screenWidth / 4, col * screenHeight / 4, screenWidth / 4, screenHeight / 4);
		glColor3f(houses[i].r, houses[i].g, houses[i].b);
		if (!houses[i].destroyed)
			parameterizedHouse(screenWidth / 2, screenHeight, houses[i].w, houses[i].h);
	}

	// draw dino
	glColor3f(1, 1, 1);
	glViewport(dinoX, dinoY, dinoW, dinoH);
	char filename[] = "dino.dat";
	drawPolyLineFile(filename);

	glutSwapBuffers(); // double buffer version of glFlush()
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case 'u': dinoY += 2; break;
	case 'r': dinoX += 2; break;
	case 'l': dinoX -= 2; break;
	case 'd': dinoY -= 2; break;
	default: break;
	}
	int row, col, houseX, houseY, houseW, houseH;
	// check for collisions after the dino moved
	for (int i = 0; i < numHouses; i++) {
		row = i / 4;
		col = i % 4;
		houseW = houses[i].w / 4;
		houseH = houses[i].h / 4;
		houseX = row * screenWidth / 4 + screenWidth / 8 - houseW / 2;
		houseY = (col + 1) * screenHeight / 4 - houseH;
		if (houseX < dinoX + dinoW &&
			houseX + houseW > dinoX &&
			houseY < dinoY + dinoH &&
			houseY + houseH > dinoY) {
			houses[i].destroyed = true;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HW 2 DinoVillage");

	glutDisplayFunc(display);
	glutKeyboardFunc(myKeyboard);

	init();
	glutMainLoop();
	return 0;
}
