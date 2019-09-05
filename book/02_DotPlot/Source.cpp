#include <windows.h> // use proper includes for your system
#include <math.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/Glut.h>
const int screenWidth = 640;	   // width of the screen window in pixels 
const int screenHeight = 480;	   // height of the screen window in pixels
GLdouble A, B, C, D;  // values used for scaling and shifting
//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);       // the background color is white
	glColor3f(0.0f, 0.0f, 0.0f);         // the drawing color is black 
	glPointSize(2.0);		           // a 'dot' is 2 by 2 pixels
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
	A = screenWidth / 4.0; // sets the values used for scaling and shifting
	B = 0.0;
	C = D = screenHeight / 2.0;
}
//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
	glBegin(GL_POINTS);
	for (GLdouble x = 0; x < 4.0; x += 0.005)
	{
		GLdouble func = exp(-x) * cos(2 * 3.14159265 * x);
		glVertex2d(A * x + B, C * func + D);
	}
	glEnd();
	glFlush();		   // send all output to display 
}
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set the display mode
	glutInitWindowSize(screenWidth, screenHeight); // set the window size
	glutInitWindowPosition(100, 150); // set the window position on screen
	glutCreateWindow("Dot Plot of a Function"); // open the screen window
	glutDisplayFunc(myDisplay);     // register the redraw function
	myInit();
	glutMainLoop(); 		     // go into a perpetual loop
}
