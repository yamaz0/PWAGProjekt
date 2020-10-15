#include <iostream>
#include <fstream>

#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glew.h"
#include "GL/freeglut.h"

using namespace std;
// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void specialKeys();
void setup();

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y = 0;
double rotate_x = 0;

void setup()
{
	float const s = 0.5;
	float cube[108] = {

		-s, -s,  s,		 s, -s,  s,		 s,  s,  s,
		-s, -s,  s,		 s,  s,  s,		-s,  s,  s,

		-s,  -s,  s,	-s, -s,  -s,	 s,  -s,  s,
		-s,  -s, -s,	 s, -s,  -s,	 s,  -s,  s,

		s, -s,  s,		 s, -s, -s,		 s,  s, -s,
		s, -s,  s,		 s,  s, -s,		 s,  s,  s,

		s,  s, -s,		 s, -s, -s,		-s, -s, -s,
		s,  s, -s,		-s, -s, -s,		-s,  s, -s,

		-s,  s, -s,		-s, -s, -s,		-s, -s,  s,
		-s,  s, -s,		-s, -s,  s,		-s,  s,  s,

		-s,  s, -s,		-s,  s,  s,		 s,  s,  s,
		-s,  s, -s,		 s,  s,  s,		 s,  s, -s
	};	
	float cubeColor[144] = {

		1,1,1,1,	1,1,1,1,	1,1,1,1,
		1,1,1,1,	1,1,1,1,	1,1,1,1,

		1,0,0,1,	1,0,0,1,	1,0,0,1,
		1,0,0,1,	1,0,0,1,	1,0,0,1,

		0,1,0,1,	0,1,0,1,	0,1,0,1,
		0,1,0,1,	0,1,0,1,	0,1,0,1,

		0,0,1,1,	0,0,1,1,	0,0,1,1,
		0,0,1,1,	0,0,1,1,	0,0,1,1,

		1,1,0,1,	1,1,0,1,	1,1,0,1,
		1,1,0,1,	1,1,0,1,	1,1,0,1,

		1,0,1,1,	1,0,1,1,	1,0,1,1,
		1,0,1,1,	1,0,1,1,	1,0,1,1
	};

	unsigned int buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), cube, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,0);
}

// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display()
{

	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Other Transformations
	// glTranslatef( 0.1, 0.0, 0.0 );      // Not included
	// glRotatef( 180, 0.0, 1.0, 0.0 );    // Not included

	// Rotate when user changes rotate_x and rotate_y
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);

	for (int i = 0; i < 12; i++)
	{
		glDrawArrays(GL_TRIANGLES, i*3, 3);
	}

	glFlush();
	glutSwapBuffers();
}

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys(int key, int x, int y)
{
	//  Right arrow - increase rotation by 5 degree
	if (key == GLUT_KEY_RIGHT)
		rotate_y += 5;

	//  Left arrow - decrease rotation by 5 degree
	else if (key == GLUT_KEY_LEFT)
		rotate_y -= 5;

	else if (key == GLUT_KEY_UP)
		rotate_x += 5;

	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 5;

	//  Request display update
	glutPostRedisplay();

}

// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[])
{

	//  Initialize GLUT and process user parameters
	glutInit(&argc, argv);

	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	// Create window
	glutCreateWindow("Awesome Cube");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	//  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
	setup();
	// Callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);

	//  Pass control to GLUT for events
	glutMainLoop();

	//  Return to OS
	return 0;

}