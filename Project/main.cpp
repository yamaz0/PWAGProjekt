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
#include "Model.h"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"

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

glm::vec3 lightPosition = glm::vec3(5.f);

std::vector<Model*> models;
std::vector<PointLight*> pointLights;

Shader *shader;
Texture *texture;
Texture *groundSpec;
Texture *groundDif;
Texture *treeTextureSpec;
Texture *treeTextureDif;

Renderer renderer;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Material* material;


void setup()
{
	//OBJ cubeObj;
	//for (size_t i = 0; i < 24; i++)
	//{
	//	glm::vec3 temp_vertices(cube[5*i], cube[5*i+1], cube[5*i+2]);
	//	glm::vec2 temp_uv(cube[5*i+3], cube[5 * i +4]);
	//	cubeObj.vertex.push_back(temp_vertices);
	//	cubeObj.uv.push_back(temp_uv);
	//}	
	//
	//for (size_t i = 0; i < 36; i++)
	//{
	//	cubeObj.indicies.push_back(indiciesCube[i]);
	//}

	//VertexBufferLayout layout1;
	//layout1.Push<float>(3);
	//layout1.Push<float>(2);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	material = new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f));
	treeTextureSpec = new Texture("Textures/treeTextureSpec.png");
	treeTextureDif = new Texture("Textures/treeTextureDif.png");
	//texture = new Texture("Textures/brown.png");
	groundDif = new Texture("Textures/groundDif.png");
	groundSpec = new Texture("Textures/groundSpec.png");
	texture = new Texture("Textures/test.png");

	std::vector<Vertex> tree = loadOBJ("OBJ/tree.obj");
	std::vector<Vertex> cube = loadOBJ("OBJ/cube.obj");

	glm::vec3 position(0, 0, 0);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			position = glm::vec3(i*2.f, 0, j * 2.f);
			models.push_back(new Model(position, material, groundDif, groundSpec, cube));
		}
	}

	position = glm::vec3(1, 0.5f, 1);
	models.push_back(new Model(position, material, treeTextureDif, treeTextureSpec, tree));
	position= glm::vec3(5, 0.5f, 1);
	models.push_back(new Model(position, material, treeTextureDif, treeTextureSpec, tree));
	position = glm::vec3(3, 0.5f, 5);
	models.push_back(new Model(position, material, treeTextureDif, treeTextureSpec, tree));


	pointLights.push_back(new PointLight(lightPosition));



	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//texture = new Texture("Textures/test.png");
	//ib = new IndexBuffer(indiciesCube, 36);
	//vb = new VertexBuffer(cube, 5 * 24 * sizeof(float));
	//ib = new IndexBuffer(indicies, obj.indicies.size());
	////ib = new IndexBuffer(&obj.indicies[0], obj.indicies.size());
	//vb = new VertexBuffer(&obj.vertex[0], obj.vertex.size() * sizeof(float));
	////vb = new VertexBuffer(cube, 11 * 12 * sizeof(float));
	//va = new VertexArray();
	shader = new Shader("Shader.shader");
	//
	//layout.Push<float>(3);
	////layout.Push<float>(2);
	//va->AddBuffer(*vb,layout);

	//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0));
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 0));
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();
/*
	glm::mat4 mvp =  *  * ;*/

	shader->Bind();
	//shader->SetUniform4f("u_Color", 0.8f, 0.8f, 0.8f, 1.0f);
	shader->SetUniformMat4f("ViewMatrix", view);
	shader->SetUniformMat4f("ProjectionMatrix", proj);
	//shader->SetUniformMat4f("ModelMatrix", (*models[0]->GetMesh())[0]->GetModelMatrix());

	//texture->Bind();
	//shader->SetUniform1i("u_Texture", 0);

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
	//glRotatef(rotate_x, 1.0, 0.0, 0.0);
	//glRotatef(rotate_y, 0.0, 1.0, 0.0);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 0));
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 mvp = proj * view * model;

	shader->Bind();
	//shader->SetUniform4f("u_Color", 0.8f, 0.8f, 0.8f, 1.0f);
	//shader->SetUniformMat4f("u_MVP", mvp);
	shader->SetUniformMat4f("ViewMatrix", view);
	shader->SetUniformMat4f("ProjectionMatrix", proj);
	//shader->SetUniformMat4f("ModelMatrix", (*models[0]->GetMesh())[0]->GetModelMatrix());

	pointLights[0]->sendToShader(*shader);

	for (int i = 0; i < models.size(); i++)
	{
		//if (i < 100)
		//	ground->Bind();
		//else
			//treeTexture->Bind();
		//shader->SetUniform1i("u_Texture", 0);
		models[i]->render(shader);
	}

	//shader->SetUniform4f("u_Color", rotate_x, 0.8f, 0.8f, 1.0f);

	//renderer.Draw(*models[0], *shader);
	//renderer.Draw(*models[1], *shader);


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
		lightPosition.z += 5;

	//  Left arrow - decrease rotation by 5 degree
	else if (key == GLUT_KEY_LEFT)
		lightPosition.z -= 5;

	else if (key == GLUT_KEY_UP)
		lightPosition.x += 5;

	else if (key == GLUT_KEY_DOWN)
		lightPosition.x -= 5;
	//  Request display update
	pointLights[0]->setPosition(lightPosition);
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

void MouseButtonHandler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		firstMouse = true;
		//store the x,y value where the click happened
		puts("Middle button clicked");
	}
}

void MouseMoveHandler(int x, int y)
{

	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

	lastX = x;
	lastY = y;

	camera.ProcessMouseMovement(xoffset, yoffset);
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
	glutCreateWindow("PWAG");

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
	glutMouseFunc(MouseButtonHandler);
	glutMotionFunc(MouseMoveHandler);
	//glutPassiveMotionFunc(MouseMoveHandler);

	//void glutMotionFunc(void(*func) (int x, int y));
	//void glutPassiveMotionFunc(void(*func) (int x, int y));

	//  Pass control to GLUT for events
	glutMainLoop();

	//delete vb;
	//delete ib;
	//delete shader;
	
	//  Return to OS
	return 0;

}