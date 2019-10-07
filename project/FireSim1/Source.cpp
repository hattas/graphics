#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

enum Tile { Grass, LightGrass, DarkGrass, Water, Fire }; // tile types
enum Color { Green, LightGreen, YellowGreen, Red, DarkOrange, Orange, DarkYellow, Yellow };

// constants
const int screenW = 600;
const int screenH = 400;
const int numTilesHoriz = 30;
const int numTilesVert = 20;
const double tileW = screenW / numTilesHoriz;
const double tileH = screenH / numTilesVert;
const double tileChance = 0.01;
const double emberChance = 0.00001;

// globals
Tile tiles[numTilesHoriz][numTilesVert];
Tile tilesPrevious[numTilesHoriz][numTilesVert];

void resetTiles() {
	for (int i = 0; i < numTilesHoriz; i++) {
		for (int j = 0; j < numTilesVert; j++) {
			if (i >= numTilesHoriz / 2 - 1 && i <= numTilesHoriz / 2 + 1 &&
				j >= numTilesVert / 2 - 1 && j <= numTilesVert / 2 + 1)
				tiles[i][j] = Fire;
			else {
				double r = (double)rand() / RAND_MAX;
				if (r < 0.95)
					tiles[i][j] = Grass;
				else if (r < 0.97)
					tiles[i][j] = LightGrass;
				else
					tiles[i][j] = DarkGrass;
			}
		}
	}
}

// set up coordinate system, point size, background color, drawing color
void myInit(void) {
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	glPointSize(2);
	glLineWidth(1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screenW, 0.0, screenH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialize tiles
	resetTiles();
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

void hexColor(int hex) {
	double r = ((hex >> 16) & 0xff) / 255.0;
	double g = ((hex >> 8) & 0xff) / 255.0;
	double b = (hex & 0xff) / 255.0;
	glColor3d(r, g, b);
}

void myColor(Color color) {
	switch (color) {
	case Green:
		hexColor(0x4caf50);
		break;
	case LightGreen:
		hexColor(0x8bc34a);
		break;
	case YellowGreen:
		hexColor(0xcddc39);
		break;
	case Red:
		hexColor(0xf44336);
		break;
	case DarkOrange:
		hexColor(0xff5722);
		break;
	case Orange:
		hexColor(0xff9800);
		break;
	case DarkYellow:
		hexColor(0xffc107);
		break;
	case Yellow:
		hexColor(0xffeb3b);
		break;
	default:
		hexColor(0);
		break;
	}
}


void setColor(Tile tile) {
	double r = (double)rand() / RAND_MAX;
	switch (tile) {
	case Fire:
		if (r < 0.5)
			myColor(DarkOrange);
		else if (r < 0.6)
			myColor(Orange);
		else
			myColor(Red);
		break;
	case Water:
		glColor3f(0, 0, 1);
		break;
	case Grass:
		myColor(LightGreen);
		break;
	case LightGrass:
		myColor(YellowGreen);
		break;
	case DarkGrass:
		myColor(Green);
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

void drawLine(double x1, double y1, double x2, double y2) {
	glBegin(GL_LINE_STRIP);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

void drawGrid() {
	glColor4f(1, 1, 1, 0.6);
	// vertical lines
	for (int i = 0; i <= numTilesHoriz; i++) {
		drawLine(i * tileW, 0, i * tileW, screenH);
	}
	// horizontal lines
	for (int i = 0; i <= numTilesVert; i++) {
		drawLine(0, i * tileH, screenW, i * tileH);
	}
}

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	renderTiles();
	drawGrid();
	glutSwapBuffers();
}

void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON)
			glutPostRedisplay();
	}
}

int checkNeighboringTiles(int i, int j, int range) {
	int count = 0;
	for (int k = i - range; k <= i + range; k++) {
		if (k < 0 || k >= numTilesHoriz)
			continue;
		for (int l = j - range; l <= j + range; l++) {
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
			int numNeighbors = checkNeighboringTiles(i, j, 1);
			int numNeighborsEmber = checkNeighboringTiles(i, j, 10);
			double chance = tileChance * numNeighbors;
			double chanceEmber = emberChance * numNeighborsEmber;
			double r = (double)rand() / RAND_MAX;
			if (r < chance) {
				tiles[i][j] = Fire;
			}
			r = (double)rand() / RAND_MAX;
			if (r < chanceEmber) {
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
		resetTiles();
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