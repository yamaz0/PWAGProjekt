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


float deltaTime = 0.1f;

std::vector<Model*> models;
std::vector<PointLight*> pointLights;
glm::vec3 lightPosition = glm::vec3(5.f);

//Camera camera(glm::vec3(5.0f, 5.0f, 16.0f));
Camera camera(glm::vec3(4.0f, 10.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Shader *shader;

Material* material;
Texture* treeTextureSpec;
Texture* treeTextureDif;
Texture* groundSpec;
Texture* groundDif;
Texture* playerSpec;
Texture* playerDif;
Player* player;


void InitializeMVP()
{
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	
	shader->Bind();
	shader->SetUniformMat4f("ProjectionMatrix", proj);
	camera.SetUniforms(shader);
}

void LoadShaders()
{
	shader = new Shader("Shader.shader");
}

void LoadMaterials()
{
	material = new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f));
}

void LoadTextures()
{
	treeTextureSpec = new Texture("Textures/treeTextureSpec.png");
	treeTextureDif = new Texture("Textures/treeTextureDif.png");
	groundSpec = new Texture("Textures/groundSpec.png");
	groundDif = new Texture("Textures/groundDif.png");
	playerSpec = new Texture("Textures/playerSpec.png");
	playerDif = new Texture("Textures/playerDif.png");
}

void LoadModels()
{
	std::vector<Vertex> treeObj = LoadOBJ("OBJ/tree.obj");
	std::vector<Vertex> cubeObj = LoadOBJ("OBJ/cube.obj");
	std::vector<Vertex> playerObj = LoadOBJ("OBJ/player.obj");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			models.push_back(new Model(glm::vec3(i*1.f, 0, j * 1.f), material, groundDif, groundSpec, cubeObj));
		}
	}

	player = new Player(new Model(glm::vec3(2.0f, 1.0f, 2.0f), material, playerDif, playerSpec, playerObj, 1));

	models.push_back(new Model(glm::vec3(1, 0.5f, 1), material, treeTextureDif, treeTextureSpec, treeObj, 1));
	models.push_back(new Model(glm::vec3(5, 0.5f, 1), material, treeTextureDif, treeTextureSpec, treeObj, 1));
	models.push_back(new Model(glm::vec3(5, 0.5f, 5), material, treeTextureDif, treeTextureSpec, treeObj, 1));
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
	camera.SetUniforms(shader);
	pointLights[0]->SetUniforms(shader);

	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Render(shader);
	}
	player->Render(shader);

	if (player->SphereRectCollision(models[102]))
	{
		std::cout << "kolizja" << std::endl;
	}

	glFlush();
	glutSwapBuffers();
}

void PlayerMove(glm::vec3 cameraDirection)
{
	glm::vec3 direction = normalize(glm::vec3(cameraDirection.x, 0, cameraDirection.z)) * deltaTime;
	camera.Move(direction);
	player->Move(direction);
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

	float deltaTime = 0.1f;

	if (key == 'w')
	{
		PlayerMove(camera.GetFront());
		//camera.ProcessKeyboard(FORWARD, deltaTime);
		//player->Move(camera.GetFront(), deltaTime);
	}
	if (key == 's')
	{
		PlayerMove(-camera.GetFront());
		//camera.ProcessKeyboard(BACKWARD, deltaTime);
		//player->Move(-camera.GetFront(), deltaTime);
	}
	if (key == 'a')
	{
		PlayerMove(-camera.GetRight());
		//camera.ProcessKeyboard(LEFT, deltaTime);
		//player->Move(-camera.GetRight(), deltaTime);
	}
	if (key == 'd')
	{
		PlayerMove(camera.GetRight());
		//camera.ProcessKeyboard(RIGHT, deltaTime);
		//player->Move(camera.GetRight(), deltaTime);
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
	delete playerSpec;
	delete playerDif;

	return 0;

}