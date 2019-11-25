#include <cmath>,
#include <vector>
#include <iostream>
#include <fstream>

#include "GlutWin.h"
#include "Camera.h"
#include "Vector3.h"

#define MOUSE_CONTROL 0

using namespace std;

typedef vector<Vector3> profile_t;
typedef vector<profile_t> surface_t;

GlutWin* win;
Camera* camera;

// constants
const int numObjects = 3;
const float material_amb[numObjects][4] = { {1, 1, 1, 1.0}, {0.7, 0.4, 0.3, 1.0}, {0.3, 0.8, 0.3, 1.0} };
const float material_dif[] = {1.0, 1.0, 0, 1.0};
const char filenames[numObjects][255] = { "bowl.dat", "plate.dat", "soup.dat" };
const int n_waist = 20;

// globals
float mouse_x, mouse_y;
bool mouse_updated = false;

profile_t profiles[numObjects];
surface_t surfaces[numObjects];

void load_profile(const char* filename, profile_t* profile) {

	ifstream i = ifstream(filename, ios_base::in);
	int n_points;
	double x, y;

	i >> n_points;

	for (int c = 0; c < n_points && !i.eof(); c++) {
		i >> x >> y;
		cout << x << " " << y << endl;
		profile->push_back(Vector3(x, y, 0.0));
	}

	i.close();
}

Vector3 matrixmult(float mat[], Vector3 v) {

	return(Vector3(mat[0] * v.x + mat[4] * v.y + mat[8] * v.z + mat[12],
		mat[1] * v.x + mat[5] * v.y + mat[9] * v.z + mat[13],
		mat[2] * v.x + mat[6] * v.y + mat[10] * v.z + mat[14]));

}

void calculate_surface(profile_t* profile, surface_t* surface) {

	const double PI = 3.141592654;
	double ang_incr = (2.0 * PI) / (double)n_waist;
	float mat[16];
	profile_t waist;
	Vector3 p;

	surface->clear();

	for (double theta = 0.0; theta <= 2 * PI; theta += ang_incr) {
		waist.clear();

		// create affine transform matrix
		// assume mat has 16 elems
		mat[0] = cos(theta); mat[4] = 0.0; mat[8] = sin(theta); mat[12] = 0.0;
		mat[1] = 0.0;		  mat[5] = 1.0; mat[9] = 0.0;		   mat[13] = 0.0;
		mat[2] = -sin(theta);mat[6] = 0.0; mat[10] = cos(theta); mat[14] = 0.0;
		mat[3] = 0.0;		  mat[7] = 0.0; mat[11] = 0.0;		   mat[15] = 1.0;

		// rotate profile
		for (int i = 0; i < profile->size(); i++) {
			p = matrixmult(mat, (*profile)[i]);
			waist.push_back(p);
		}

		// add to surface
		surface->push_back(waist);
	}

}

// set camera viewing positions
void set_camera(int position) {
	float ex, ey, ez, lx, ly, lz, upx, upy, upz;

	// camera position
	ex = 10 + position * 10;
	ey = 5 + position * 5;
	ez = 10;

	// look point - center of scene
	lx = 0;
	ly = 10;
	lz = 0;

	// camera orientation - this is regular/not rotated
	upx = 0;
	upy = 1;
	upz = 0;

	camera->set(ex, ey, ez, lx, ly, lz, upx, upy, upz);
}

bool initdemo() {

	// initialize GLUT class
	win = new GlutWin(600, 800,
		100, 100,
		GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH,
		"Surface Of Revolution Demo");


	camera = new Camera();
	set_camera(2);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < numObjects; i++) {
		load_profile(filenames[i], &profiles[i]);
		calculate_surface(&profiles[i], &surfaces[i]);
	}

	// set up lighting in ogl
	glEnable(GL_LIGHTING);

	float light_amb[] = { 0.5, 0.5, 0.5, 1.0 };
	float light_dif[] = { 0.25, 0.25, 0.25, 1.0 };
	float light_pos[] = { 10.0, 10.0, 10.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glEnable(GL_LIGHT0);

	cout << "init done" << endl;

	return(true);
}

void renderSurface(surface_t* surface) {
	glBegin(GL_QUADS);

	for (int w = 0; w < surface->size(); w++) {

		profile_t curr_waist = (*surface)[w];
		profile_t next_waist = (*surface)[(w + 1) % surface->size()];

		// draw each waist connected to the next
		for (int p = 0; p < (*surface)[w].size() - 1; p++) {
			Vector3 p0, p1, p2, p3, norm;

			p0 = curr_waist[p];
			p1 = next_waist[p];
			p2 = next_waist[p + 1]; // failing here
			p3 = curr_waist[p + 1];

			// find per surface normal (eqn 6.1)
			Vector3 t1 = (p1 - p0);
			Vector3 t2 = (p1 + p0);

			norm.x = t1.y * t2.z;
			norm.y = t1.z * t2.x;
			norm.z = t1.x * t2.y;

			t1 = (p2 - p1);
			t2 = (p2 + p1);

			norm.x += t1.y * t2.z;
			norm.y += t1.z * t2.x;
			norm.z += t1.x * t2.y;

			t1 = (p3 - p2);
			t2 = (p3 + p2);

			norm.x += t1.y * t2.z;
			norm.y += t1.z * t2.x;
			norm.z += t1.x * t2.y;

			norm.normalize();

			// send to ogl
			glNormal3f(norm.x, norm.y, norm.z);

			glVertex3f(p0.x, p0.y, p0.z);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);
		}

	}

	glEnd();
}

void render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

	for (int i = 0; i < numObjects; i++) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_amb[i]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif);

		renderSurface(&surfaces[i]);
	}

	// refresh image
	glutSwapBuffers();
}

void kb_input(unsigned char key, int x, int y) {

	switch (key) {

	case 'a': camera->slide(-1.0, 0.0, 0.0); break;
	case 'd': camera->slide(1.0, 0.0, 0.0); break;
	case 's': camera->slide(0.0, 0.0, 1.0); break;
	case 'w': camera->slide(0.0, 0.0, -1.0); break;
	case 'q': exit(0); break;
	default:	break;
	}

	if (key >= '0' && key <= '9') {
		if (key == '0') // make 0 into 10 to reflect keyboard order
			key += 10;
		set_camera(key - '0');
	}

	glutPostRedisplay();
}

void mouse_motion(int x, int y) {

	float dx, dy;

	if (!mouse_updated) {
		mouse_x = x;
		mouse_y = y;

		mouse_updated = true;
	}
	else {
		// calc delta
		dx = x - mouse_x;
		dy = y - mouse_y;

		// invert vertical
		dy *= -1;

		mouse_x = x;
		mouse_y = y;

		// determine rotation amount

		camera->pitch(dy / 5.0);
		camera->yaw(dx);
	}

}

int main() {

	// init
	if (initdemo()) {
		glutDisplayFunc(render);
		glutKeyboardFunc(kb_input);
#if MOUSE_CONTROL
		glutPassiveMotionFunc(mouse_motion);
#endif

		glutMainLoop();

		delete win;
		delete camera;
	}

}
