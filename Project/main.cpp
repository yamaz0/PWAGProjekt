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

//static int programHandle; // obiekt programu
//static int vertexShaderHandle; // obiekt shadera wierzcho�k�w
//static int fragmentShaderHandle; // obiekt shadera fragment�w
//static GLint locMVP;  // macierz przekszta�cenia

VertexBuffer *vb;
IndexBuffer *ib;
VertexArray *va;
Shader *shader;
Texture *texture;
VertexBufferLayout layout;

Renderer renderer;

const char* vs = "vert.vs";
const char* fs = "frag.fs";

float positions[8] =
{
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f,0.5f
};

unsigned int indicies[6] =
{
	0,1,2,
	2,3,0
};
float const s = 0.5;
float cubeT[108] = {

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
unsigned int indiciesCubeT[36] = {

	0,1,2,
	3,4,5,
	6,7,8,
	9,10,11,
	12,13,14,
	15,16,17,
	18,19,20,
	21,22,23,
	24,25,26,
	27,28,29,
	30,31,32,
	33,34,35
};float cube[] = {
	-s, -s,  s,
	 s, -s,  s,
	 s,  s,  s,
	-s,  s,  s,

	-s, -s, -s,
	-s,  s, -s,
	 s,  s, -s,
	 s, -s, -s,

	-s,  s, -s,
	-s,  s,  s,
	 s,  s,  s,
	 s,  s, -s,

	-s, -s, -s,
	 s, -s, -s,
	 s, -s,  s,
	-s, -s,  s,

	 s, -s, -s,
	 s,  s, -s,
	 s,  s,  s,
	 s, -s,  s,

	-s, -s, -s,
	-s, -s,  s,
	-s,  s,  s,
	-s,  s, -s

};
unsigned int indiciesCube[] = {
	0

};


//// funkcja do odczytu kodu shaderow
//char* readShader(const char* aShaderFile)
//{
//	FILE* filePointer = fopen(aShaderFile, "rb");
//	char* content = NULL;
//	long numVal = 0;
//
//	fseek(filePointer, 0L, SEEK_END);
//	numVal = ftell(filePointer);
//	fseek(filePointer, 0L, SEEK_SET);
//	content = (char*)malloc((numVal + 1) * sizeof(char));
//	fread(content, 1, numVal, filePointer);
//	content[numVal] = '\0';
//	fclose(filePointer);
//	return content;
//}
//
//
//
//// incjalizacja shader�w
//void setShaders(const char* vertexShaderFile, const char* fragmentShaderFile)
//{
//	GLint status = 0;
//
//	char* vertexShader = readShader(vertexShaderFile);
//	char* fragmentShader = readShader(fragmentShaderFile);
//
//	programHandle = glCreateProgram(); // tworzenie obiektu programu
//	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER); // shader wierzcho�k�w
//	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER); // shader fragment�w
//
//
//	glShaderSource(vertexShaderHandle, 1, (const char**)&vertexShader, NULL); // ustawianie �r�d�a shadera wierzcho�k�w
//	glShaderSource(fragmentShaderHandle, 1, (const char**)&fragmentShader, NULL); // ustawianie �r�d�a shadera fragment�w
//
//	   // kompilacja shader�w
//	glCompileShader(vertexShaderHandle);
//	glCompileShader(fragmentShaderHandle);
//
//	char infoLog[512];
//	glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &status);
//
//	if (!status)
//	{
//		const int maxInfoLogSize = 2048;
//		GLchar infoLog[maxInfoLogSize];
//		glGetInfoLogARB(fragmentShaderHandle, maxInfoLogSize, NULL, infoLog);
//		std::cout << infoLog;
//	}
//
//	//dodanie shader�w do programu
//	glAttachShader(programHandle, vertexShaderHandle);
//	glAttachShader(programHandle, fragmentShaderHandle);
//
//
//	/* link */
//	//uruchomienie
//	glLinkProgram(programHandle);
//	glGetObjectParameterivARB(programHandle, GL_OBJECT_LINK_STATUS_ARB, &status);
//	if (!status) {
//		const int maxInfoLogSize = 2048;
//		GLchar infoLog[maxInfoLogSize];
//		glGetInfoLogARB(programHandle, maxInfoLogSize, NULL, infoLog);
//		std::cout << infoLog;
//	}
//	glUseProgram(programHandle); // Installs program into current rendering state.
//
//	//zmienna typu UNIFORM -- macierz przekszta�cenia
//	locMVP = glGetUniformLocation(programHandle, "MVP");
//}

void setup()
{
	texture = new Texture("Textures/test.png");
	ib = new IndexBuffer(indiciesCube,36);
	vb = new VertexBuffer(cube, 108 * sizeof(float));
	va = new VertexArray();
	shader = new Shader("Basic.shader");

	
	layout.Push<float>(3);
	va->AddBuffer(*vb,layout);

	shader->Bind();
	shader->SetUniform4f("u_Color", 0.8f, 0.8f, 0.8f, 1.0f);

	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	va->Unbind();
	shader->Unbind();
	vb->Unbin();
	ib->Unbin();
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

	//setShaders(vs, fs);
	// Callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);

	//  Pass control to GLUT for events
	glutMainLoop();

	//delete vb;
	//delete ib;
	//delete shader;
	
	//  Return to OS
	return 0;

}