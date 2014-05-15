
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#pragma once

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

#include "Algebra3.h"
#include "Joint.h"
#include "IKSolver.h"

using namespace std;
/* ************************************************************************* */

/* ******************************** Classes ******************************** */
class Viewport {
public:
	int w, h;				// Width and height
};
/* ************************************************************************* */

/* *************************** Global Variables **************************** */
Viewport	viewport;

// Navigation variables for camera rotation and position.
static float zoom = 0.0f;
static float rotx = 0.0f, roty = 0.0f;
static float tx = 0.0f, ty = 0.0f;

// Arm components.
Joint joint1 = Joint(vec3(0, 0, 0), vec3(-0.2, 0.2, 0.2));
Joint j2 = Joint(&joint1, vec3(-0.3, 0.5, -0.5));
Joint j3 = Joint(&j2, vec3(0.3, 1.3, -0.3));
Joint j4 = Joint(&j3, vec3(0.4, 0.7, 0.7));

// Coordinates of goal x, y, z
float goal[3] = {0.0f, 0.0f, 0.0f};
/* ************************************************************************* */

/* ************************************************************************* */
/* Resize the projection
/* ************************************************************************* */
void cameraMotion() {
	// Rotate and zoom the camera
	glTranslatef(0, 0, zoom);
	glTranslatef(tx, ty, 0);
	glRotatef(rotx, 1, 0, 0);
	glRotatef(roty, 0, 1, 0);
};

void resizeProjection() {

	glMatrixMode(GL_PROJECTION);
	// loading the identity matrix for the screen
	glLoadIdentity();

	//----------- Setting the projection -------------------------
	// glOrtho sets left, right, bottom, top, zNear, zFar of the chord system
	float radius = 2.0;
	//glOrtho(-2*radius, 2*radius, -2*radius, 2*radius, .01, 4*radius);	// resize type = stretch

	// gluPerspective(fieldofview, aspectratio, nearplane, farplane);
	// we need to use gluPerspective for zoom to work.
	gluPerspective(45.0, 1.0, 1.0, 100.0);
	//------------------------------------------------------------
	
	// gluLookAt(camera position, center of viewport, up-direction)
	gluLookAt(0,0,4*radius, 0,0,0, 0,1,0);

    glMatrixMode(GL_MODELVIEW);
	cameraMotion();
};

/* ************************************************************************* */
/* Reshape viewport if the window is resized.
/* ************************************************************************* */
void myReshape(int w, int h) {
	viewport.w = w;
	viewport.h = h;

	// Sets the rectangle that will be the window.
	glViewport(0, 0, viewport.w, viewport.h);

	resizeProjection();
}

/* ************************************************************************* */
/* Create and initialize the scene.
/* ************************************************************************* */
void initScene(){
	// Clear to black, fully transparent
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
    glEnable(GL_DEPTH_TEST);

	myReshape(viewport.w, viewport.h);
}

// Global variables for animation
int curr = 0;
double radius = 2.0f;
double zCoord = 2.0f;

int maxSamples = 200;
double xCoord = 0.0f;
double yCoord = 0.0f;
double tCoord = 0.0f;
// double increment = ((4.0f * radius) / ((double) maxSamples));
double increment = 0.02f;

/* ************************************************************************* */
/* Handle SPACEBAR keyboard stroke to exit.
/* ************************************************************************* */
void keyboard (unsigned char key, int x, int y) {
	switch (key) {
		// SPACEBAR key
		case ' ' :
			// End the program
			exit(0);
		case '+' :
			zoom += 0.2;
			cameraMotion();
			zoom -= 0.2;
			break;
		case '-' :
			zoom -= 0.2;
			cameraMotion();
			zoom += 0.2;
			break;
		case 'u' :
			/* xCoord = xCoord + increment;
			if (xCoord > radius)		xCoord = radius;
			if (xCoord < -1 * radius)	xCoord = -1 * radius;

			yCoord = (double) sqrt((radius * radius) - (xCoord * xCoord));
			if (increment < 0) yCoord = -1 * yCoord;

			IKSolver::performCCD(&j4, vec3(xCoord, yCoord, zCoord));

			curr++;
			if ((curr == maxSamples/2) || (curr == maxSamples))
				increment = -1 * increment;
			if (curr == maxSamples) curr = 0; */
			break;
		case 'v':
			IKSolver::performCCD(&j4, vec3(goal[0], goal[1], goal[2]));
			break;
    }
};

/* ************************************************************************* */
/* Draws a circle with the given center and radius.                          */
/* Inspired by http://www.codeguru.com/forum/archive/index.php/t-140625.html */
/* ************************************************************************* */
void drawCircle (float xc, float yc, float zc, int rad)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0.5f, 0.5f, 0.0f);

	for (int angle = 0; angle < 370; angle = angle + 5) {
		float angle_radians = ((float) angle) * PI / (float) 180;
		float x = xc + (float) rad * (float) cos(angle_radians);
		float y = yc + (float) rad * (float) sin(angle_radians);
		glVertex3f(x, y, zc);
	}

	glEnd();
};

void processSpecialKeys (int key, int x, int y) {
	// detect ctrl, alt, shift
	int mod = glutGetModifiers();

	if (mod == GLUT_ACTIVE_SHIFT) {
		// translate
		switch (key) {
			case GLUT_KEY_LEFT :
				tx -= 0.2;
				cameraMotion();
				tx += 0.2;
				break;
			case GLUT_KEY_RIGHT :
				tx += 0.2;
				cameraMotion();
				tx -= 0.2;
				break;
			case GLUT_KEY_UP :
				ty += 0.2;
				cameraMotion();
				ty -= 0.2;
				break;
			case GLUT_KEY_DOWN :
				ty -= 0.2;
				cameraMotion();
				ty += 0.2;
				break;
		}
	} else {
		// rotate
		switch (key) {
			case GLUT_KEY_LEFT :
				roty += 2;
				cameraMotion();
				roty -= 2;
				break;
			case GLUT_KEY_RIGHT :
				roty -= 2;
				cameraMotion();
				roty += 2;
				break;
			case GLUT_KEY_UP :
				rotx += 2;
				cameraMotion();
				rotx -= 2;
				break;
			case GLUT_KEY_DOWN :
				rotx -= 2;
				cameraMotion();
				rotx += 2;
				break;
		}
	}
};

/* ************************************************************************* */
/* Initialize Light Sources
/* (Refer to handout from section)
/* ************************************************************************* */
void initLights() {

	// define and enable light0. You have 8 lights in total.
	GLfloat global_ambient[] = { .1f, .1f, .1f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	GLfloat ambient[] = { .1f, .1f, .1f };
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};		// Red diffuse light
	GLfloat specular[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat pos0[] = {1.0, 1.0, 1.0, 0.0};			// Infinite light location.
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glEnable(GL_LIGHT0);

	GLfloat pos1[] = {-1.0, -1.0, -1.0, 0.0};
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glEnable(GL_LIGHT1);
	
    glEnable(GL_LIGHTING);

	// define material properties:
	// You probably don't want to use the emission term for this class.
	GLfloat mat_specular[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat mat_diffuse[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat mat_ambient[] = {0.0, 0.1, 0.1, 1.0};
	GLfloat mat_emission[] = {0.2, 1.0, 0.0, 1.0};
	GLfloat mat_shininess = {10.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
};

/* ************************************************************************* */
/* Perform the actual drawing.
/* ************************************************************************* */

void draw() {

	glLineWidth(5);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(joint1.base.getX(), joint1.base.getY(), joint1.base.getZ());
	glVertex3f(joint1.tip.getX(), joint1.tip.getY(), joint1.tip.getZ());
	glEnd();
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(joint1.base[0], joint1.base[1], joint1.base[2]);
	glutSolidSphere(0.08, 10, 10);
	glPopMatrix();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(j2.base.getX(), j2.base.getY(), j2.base.getZ());
	glVertex3f(j2.tip.getX(), j2.tip.getY(), j2.tip.getZ());
	glEnd();
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(j2.base[0], j2.base[1], j2.base[2]);
	glutSolidSphere(0.08, 10, 10);
	glPopMatrix();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(j3.base.getX(), j3.base.getY(), j3.base.getZ());
	glVertex3f(j3.tip.getX(), j3.tip.getY(), j3.tip.getZ());
	glEnd();
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(j3.base[0], j3.base[1], j3.base[2]);
	glutSolidSphere(0.08, 10, 10);
	glPopMatrix();

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(j4.base.getX(), j4.base.getY(), j4.base.getZ());
	glVertex3f(j4.tip.getX(), j4.tip.getY(), j4.tip.getZ());
	glEnd();
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(j4.base[0], j4.base[1], j4.base[2]);
	glutSolidSphere(0.08, 10, 10);
	glPopMatrix();

	//glPushMatrix();
    //glMatrixMode(GL_MODELVIEW);
	//glTranslatef(goal[0], goal[1], goal[2]);
	// glTranslated(goal.getX(), goal.getY(), goal.getZ());
	//glutSolidSphere(0.05, 10, 10);
//	glutSolidTeapot(0.10);
	//glPopMatrix();
};

void myDisplay() {

	// clear the color buffer and z-buffer (sets everything to black)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// initLights();

	//----------------------- code to draw objects --------------------------
	draw();

	drawCircle(0.0f, 0.0f, zCoord, radius);

	// glutSolidTeapot(1.50);
	//-----------------------------------------------------------------------

	glFlush();

	// swap buffers (we earlier set double buffer)
	glutSwapBuffers();
};

//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {

	float dt;
	// Compute the time elapsed since the last time the scence is redrawn
#ifdef _WIN32
//	Sleep(10);						//give ~10ms back to OS (so as not to waste the CPU)
	DWORD currentTime = GetTickCount();
	dt = (float)(currentTime - lastTime)*0.001f; 
#else
	timeval currentTime;
	gettimeofday(&currentTime, NULL);
	dt = (float)((currentTime.tv_sec - lastTime.tv_sec) + 1e-6*(currentTime.tv_usec - lastTime.tv_usec));
#endif
	
	// Store the time
	lastTime = currentTime;

	//if (dt < 0.01) { //100 frames per second
	//	Sleep(10); //~10ms
	//}
	
	tCoord = tCoord + increment;
	xCoord = radius * cos(tCoord);
	yCoord = radius * sin(tCoord);

	/* xCoord = xCoord + increment;
		if (xCoord > radius)		xCoord = radius;
		if (xCoord < -1 * radius)	xCoord = -1 * radius;

		yCoord = (double) sqrt((radius * radius) - (xCoord * xCoord));
		if (increment < 0) yCoord = -1 * yCoord; */
	IKSolver::performCCD(&j4, vec3(xCoord, yCoord, zCoord));

	/*	curr++;
		if ((curr == maxSamples/2) || (curr == maxSamples))
			increment = -1 * increment;
		if (curr == maxSamples) curr = 0; */

	// Re-display
	glutPostRedisplay();
};

/* ************************************************************************* */
/* Main method.
/* ************************************************************************* */
int main(int argc, char *argv[]) {

  	//This initializes glut
  	glutInit(&argc, argv);
  
  	//This tells glut to use a double-buffered window with red, green, and blue channels
	//  also turn on z-buffering
  	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  	// Initalize theviewport size
  	viewport.w = 400;
  	viewport.h = 400;

  	//The size and position of the window
  	glutInitWindowSize(viewport.w, viewport.h);
  	glutInitWindowPosition(0, 0);
  	glutCreateWindow("As6!");

   	// Initialize timer variable
	#ifdef _WIN32
	lastTime = GetTickCount();
	#else
	gettimeofday(&lastTime, NULL);
	#endif 	

	// quick function to set up scene
	// initLights();
    initScene();
  
	// function to run when its time to draw something
  	glutDisplayFunc(myDisplay);

	// function to run when the window gets resized
  	glutReshapeFunc(myReshape);

	// function to run when not handling any other task
  	glutIdleFunc(myFrameMove);

	// Function to run when keyboard is hit.
	// We referred to: http://www.swiftless.com/tutorials/opengl/keyboard.html
	glutKeyboardFunc(keyboard);
	// Also referred to: http://www.lighthouse3d.com/opengl/glut/index.php?5
	glutSpecialFunc(processSpecialKeys);

	// infinite loop that will keep drawing and resizing and whatever else
  	glutMainLoop();

  	return 0;
};