#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

enum Tile { Grass, Water, Fire }; // tile types
enum Color { GreenLight, GreenMed, GreenDark, RedLight, RedMed, RedDark, OrangeDark, OrangeLight, YellowDark }; // tile types

// constants
const int screenW = 600;
const int screenH = 400;
const int numTilesHoriz = 60;
const int numTilesVert = 40;
const double tileW = screenW / numTilesHoriz;
const double tileH = screenH / numTilesVert;
const double tileChance = 0.02;

// globals
Tile tiles[numTilesHoriz][numTilesVert];
Tile tilesPrevious[numTilesHoriz][numTilesVert];


// set up coordinate system, point size, background color, drawing color
void myInit(void) {
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	glPointSize(2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screenW, 0.0, screenH);

	// initialize tiles
	for (int i = 0; i < numTilesHoriz; i++) {
		for (int j = 0; j < numTilesVert; j++) {
			tiles[i][j] = Grass;
		}
	}
	tiles[numTilesHoriz / 2][numTilesVert / 2] = Fire;
}

void drawTile(double left, double bottom) {
	double right = left + tileW;
	double top = bottom + tileH;
	glBegin(GL_POLYGON);
	glVertex2i(left, bottom);
	glVertex2i(right, bottom);
	glVertex2i(right, top);
	glVertex2i(left, top);
	glEnd();
}

void setColor(Tile tile) {
	switch (tile) {
	case Fire:
		glColor3f(1, 0, 0);
		break;
	case Water:
		glColor3f(0, 0, 1);
		break;
	case Grass:
		glColor3f(0, 1, 0);
		break;
	default:
		glColor3f(0, 0, 0);
		break;
	}
}

void renderTiles(void) {
	for (int i = 0; i < numTilesHoriz; i++) {
		for (int j = 0; j < numTilesVert; j++) {
			setColor(tiles[i][j]);
			drawTile(i * tileW, j * tileH);
		}
	}
}

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	//drawGrid();
	renderTiles();
	glutSwapBuffers();
}

void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON)
			glutPostRedisplay();
	}
}

int checkNeighboringTiles(int i, int j) {
	int count = 0;
	for (int k = i - 1; k <= i + 1; k++) {
		if (k < 0 || k >= numTilesHoriz)
			continue;
		for (int l = j - 1; l <= j + 1; l++) {
			if (l < 0 || l >= numTilesVert || (i == k && j == l))
				continue;
			if (tilesPrevious[k][l] == Fire) {
				count++;
			}
		}
	}
	return count;
}

void spreadFire() {
	// copy tiles to base fire spreading off previous state
	for (int i = 0; i < numTilesHoriz; i++) {
		for (int j = 0; j < numTilesVert; j++) {
			tilesPrevious[i][j] = tiles[i][j];
		}
	}


	for (int i = 0; i < numTilesHoriz; i++) {
		for (int j = 0; j < numTilesVert; j++) {
			if (tilesPrevious[i][j] == Fire)
				continue;
			int numNeighbors = checkNeighboringTiles(i, j);
			double chance = tileChance * numNeighbors;
			double r = (double) rand() / RAND_MAX;
			if (r < chance) {
				tiles[i][j] = Fire;
			}
		}
	}
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	switch (theKey) {
	case 'f': 
		spreadFire();
		glutPostRedisplay();
		break;
	case 'r':
		for (int i = 0; i < numTilesHoriz; i++) {
			for (int j = 0; j < numTilesVert; j++) {
				tiles[i][j] = Grass;
			}
		}
		tiles[numTilesHoriz / 2][numTilesVert / 2] = Fire;
		glutPostRedisplay();
	default: break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Fire Simulation");

	// register the callback functions
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	myInit();
	glutMainLoop();
}