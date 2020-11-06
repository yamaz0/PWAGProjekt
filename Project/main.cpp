#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#define _CRT_SECURE_NO_DEPRECATE
#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "OBJLoader.h"
#include "Camera.h"

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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

double rotate_y = 0;
double rotate_x = 0;

VertexBuffer *vb;
IndexBuffer *ib;
VertexArray *va;
Shader *shader;
Texture *texture;
VertexBufferLayout layout;

Renderer renderer;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float const s = 0.5;

float cube[] = {
	-s, -s,  s, 0, 0, //0
	 s, -s,  s, 1, 0, //1
	 s,  s,  s, 1, 1, //2
	-s,  s,  s, 0, 1, //3

	-s, -s, -s, 0, 0,
	-s,  s, -s, 1, 0,
	 s,  s, -s, 1, 1,
	 s, -s, -s, 0, 1,

	-s,  s, -s, 0, 0,
	-s,  s,  s, 1, 0,
	 s,  s,  s, 1, 1,
	 s,  s, -s, 0, 1,

	-s, -s, -s, 0, 0,
	 s, -s, -s, 1, 0,
	 s, -s,  s, 1, 1,
	-s, -s,  s, 0, 1,

	 s, -s, -s, 0, 0,
	 s,  s, -s, 1, 0,
	 s,  s,  s, 1, 1,
	 s, -s,  s, 0, 1,

	-s, -s, -s, 0, 0,
	-s, -s,  s, 1, 0,
	-s,  s,  s, 1, 1,
	-s,  s, -s, 0, 1
	};

unsigned int indiciesCube[] = {
	0,1,2,
	0,2,3,

	4,5,6,
	4,6,7,

	8,9,10,
	8,10,11,

	12,13,14,
	12,14,15,

	16,17,18,
	16,18,19,

	20,21,22,
	20,22,23
	};

void setup()
{
	//OBJ obj = loadOBJ("OBJ/lowpolytree.obj");//????????????

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	texture = new Texture("Textures/test.png");
	ib = new IndexBuffer(indiciesCube, 36);
	vb = new VertexBuffer(cube, 5 * 24 * sizeof(float));
	//vb = new VertexBuffer(cube, 11 * 12 * sizeof(float));
	va = new VertexArray();
	shader = new Shader("Basic.shader");
	
	layout.Push<float>(3);
	layout.Push<float>(2);
	va->AddBuffer(*vb,layout);

	//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 0));
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 mvp = proj * view * model;

	shader->Bind();
	//shader->SetUniform4f("u_Color", 0.8f, 0.8f, 0.8f, 1.0f);
	shader->SetUniformMat4f("u_MVP", mvp);

	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	//va->Unbind();
	//shader->Unbind();
	//vb->Unbin();
	//ib->Unbin();
}

// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display()
{
	//  Clear screen and Z-buffer
	renderer.Clear();

	// Reset transformations
	glLoadIdentity();

	// Other Transformations
	// glTranslatef( 0.1, 0.0, 0.0 );      // Not included
	// glRotatef( 180, 0.0, 1.0, 0.0 );    // Not included

	// Rotate when user changes rotate_x and rotate_y
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);


	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 0));
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 mvp = proj * view * model;

	shader->Bind();
	//shader->SetUniform4f("u_Color", 0.8f, 0.8f, 0.8f, 1.0f);
	shader->SetUniformMat4f("u_MVP", mvp);



	shader->SetUniform4f("u_Color", rotate_x, 0.8f, 0.8f, 1.0f);

	renderer.Draw(*va, *ib, *shader);

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
		rotate_x += 0.1f;

	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 0.1f;
	//  Request display update
	glutPostRedisplay();

}

void NormalKeyHandler(unsigned char key, int x, int y)
{
	if (key == 'o')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == 'p')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == 'w')
		camera.ProcessKeyboard(FORWARD, 0.1f);
	if (key == 's')
		camera.ProcessKeyboard(BACKWARD, 0.1f);
	if (key == 'a')
		camera.ProcessKeyboard(LEFT, 0.1f);
	if (key == 'd')
		camera.ProcessKeyboard(RIGHT, 0.1f);

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
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
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

	//setShaders(vs, fs);
	// Callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(NormalKeyHandler);

	//  Pass control to GLUT for events
	glutMainLoop();

	//delete vb;
	//delete ib;
	//delete shader;
	
	//  Return to OS
	return 0;

}