#include <iostream>
#include <vector>
#include <string>

#define _CRT_SECURE_NO_DEPRECATE
#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Model.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Player.h"

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::vector<Model*> models;
std::vector<PointLight*> pointLights;
glm::vec3 lightPosition = glm::vec3(5.f);

Camera camera(glm::vec3(5.0f, 5.0f, 16.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Shader *shader;

Material* material;
Texture* treeTextureSpec;
Texture* treeTextureDif;
Texture* groundDif;
Texture* groundSpec;
Player* player;


void InitializeMVP()
{
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	
	shader->Bind();
	shader->SetUniformMat4f("ProjectionMatrix", proj);
	camera.SetUniforms(*shader);
}

void LoadShaders()
{
	shader = new Shader("Shader.shader");
}

void LoadModels()
{
	std::vector<Vertex> tree = LoadOBJ("OBJ/tree.obj");
	std::vector<Vertex> cube = LoadOBJ("OBJ/cube.obj");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			models.push_back(new Model(glm::vec3(i*2.f, 0, j * 2.f), material, groundDif, groundSpec, cube));
		}
	}
	player = new Player(new Model(glm::vec3(5.0f, 3.0f, 16.0f), material, groundDif, groundSpec, cube));

	models.push_back(new Model(glm::vec3(1, 0.5f, 1), material, treeTextureDif, treeTextureSpec, tree));
	models.push_back(new Model(glm::vec3(5, 0.5f, 1), material, treeTextureDif, treeTextureSpec, tree));
	models.push_back(new Model(glm::vec3(3, 0.5f, 5), material, treeTextureDif, treeTextureSpec, tree));
}

void LoadMaterials()
{
	material = new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f));
}

void LoadTextures()
{
	treeTextureSpec = new Texture("Textures/treeTextureSpec.png");
	treeTextureDif = new Texture("Textures/treeTextureDif.png");
	groundDif = new Texture("Textures/groundDif.png");
	groundSpec = new Texture("Textures/groundSpec.png");
}

void Setup()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	LoadMaterials();
	LoadTextures();
	LoadModels();

	pointLights.push_back(new PointLight(lightPosition));

	LoadShaders();

	InitializeMVP();
}

// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void Display()
{
	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Bind();
	camera.SetUniforms(*shader);
	pointLights[0]->SetUniforms(*shader);

	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Render(shader);
	}
	player->Render(shader);

	glFlush();
	glutSwapBuffers();
}

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
		lightPosition.z += 5;
	else if (key == GLUT_KEY_LEFT)
		lightPosition.z -= 5;
	else if (key == GLUT_KEY_UP)
		lightPosition.x += 5;
	else if (key == GLUT_KEY_DOWN)
		lightPosition.x -= 5;

	pointLights[0]->SetPosition(lightPosition);

	//  Request display update
	glutPostRedisplay();
}
// ----------------------------------------------------------
// NormalKeyHandler() Callback Function
// ----------------------------------------------------------
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
	{
		camera.ProcessKeyboard(FORWARD, 0.4f);
		player->Move(camera.GetFront(), 0.1f);
	}
	if (key == 's')
	{
		camera.ProcessKeyboard(BACKWARD, 0.4f);
		player->Move(-camera.GetFront(), 0.1f);
	}
	if (key == 'a')
	{
		camera.ProcessKeyboard(LEFT, 0.4f);
		player->Move(-camera.GetRight(), 0.1f);
	}
	if (key == 'd')
	{
		camera.ProcessKeyboard(RIGHT, 0.4f);
		player->Move(camera.GetRight(), 0.1f);
	}

	glutPostRedisplay();
}

void MouseButtonHandler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		firstMouse = true;
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
	Setup();

	// Callback functions
	glutDisplayFunc(Display);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(NormalKeyHandler);
	glutMouseFunc(MouseButtonHandler);
	glutMotionFunc(MouseMoveHandler);
	//glutPassiveMotionFunc(MouseMoveHandler);
	
	//  Pass control to GLUT for events
	glutMainLoop();

	delete shader;
	delete material;
	delete treeTextureSpec;
	delete treeTextureDif;
	delete groundDif;
	delete groundSpec;
	return 0;

}