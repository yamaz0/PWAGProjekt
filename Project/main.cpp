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
#include "GameObject.h"
#include "Obstacle.h"


void PlayerMove(glm::vec3 cameraDirection);

void RestartPlayerPosition();

void Win();

// ----------------------------------------------------------
// Variables	
// ----------------------------------------------------------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const float CAMERA_Y = 8.0f;

const int MAP_SIZE = 12;
const int MAP_GROUND = 1;
const int MAP_BOUND = -1;
const int MAP_START = 1;
const int MAP_TREE = 2;
const int MAP_OBSTACLE = 3;
const int MAP_END = 9;

const float LIGHT_AMBIENT = 0.98f;
const float LIGHT_DIFFUSE = 1.f;
const float LIGHT_SPECULAR = 1.f;

float deltaTime = 0.0f;
float time = 0.0f;
glm::vec3 playerMoveDirection;

std::vector<Model*> groundObjects;
std::vector<GameObject*> gameObjects;
std::vector<GameObject*> boundsObjects;
std::vector<Obstacle*> obstacleObjects;
std::vector<PointLight*> pointLights;
glm::vec3 lightPosition = glm::vec3(0.f);
glm::vec3 startPosition = glm::vec3(0.f);

Camera* camera;
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
GameObject* end;
Model* win;

int mapGrounds[MAP_SIZE][MAP_SIZE] =
{
	{0,0,0,0,0,0,0,0,0,0,0,0},//0
	{0,1,1,1,1,1,1,1,1,1,1,0},//1
	{0,1,1,0,0,0,0,0,0,1,1,0},//2
	{0,1,1,0,1,0,0,1,0,1,1,0},//3
	{0,1,1,0,0,0,0,0,0,1,1,0},//4
	{0,1,1,0,0,0,0,0,0,1,1,0},//5
	{0,1,1,0,1,0,0,1,0,1,1,0},//6
	{0,1,1,0,0,0,0,0,0,1,1,0},//7
	{0,1,1,0,0,0,0,0,0,1,1,0},//8
	{0,1,1,0,1,0,0,1,0,1,1,0},//9
	{0,1,1,0,0,0,0,0,0,1,1,0},//9
	{0,0,0,0,0,0,0,0,0,0,0,0}//10
};

int mapObjects[MAP_SIZE][MAP_SIZE] =
{
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},//0
	{-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1},//1
	{-1, 0, 0,-1,-1,-1,-1,-1,-1, 0, 0,-1},//2
	{-1, 2, 0,-1, 2, 0, 0, 2,-1, 0, 2,-1},//3
	{-1, 0, 0,-1, 0, 0, 0, 0,-1, 0, 3,-1},//4
	{-1, 0, 0,-1, 0, 0, 0, 0,-1, 0, 0,-1},//5
	{-1, 0, 3,-1, 2, 0, 0, 2,-1, 0, 0,-1},//6
	{-1, 0, 0,-1, 0, 0, 0, 0,-1, 0, 0,-1},//7
	{-1, 0, 3,-1, 0, 0, 0, 0,-1, 0, 0,-1},//8
	{-1, 0, 0,-1, 2, 0, 0, 2,-1, 0, 3,-1},//9
	{-1, 1, 0,-1, 0, 0, 0, 0,-1, 0, 9,-1},//10
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} //11
};

void InitializeMVP()
{
	glm::mat4 proj = glm::perspective(glm::radians(camera->GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	shader->Bind();
	shader->SetUniformMat4f("ProjectionMatrix", proj);
	camera->SetUniforms(shader);
}

void LoadShaders()
{
	shader = new Shader("Shader.shader");
}

void LoadMaterials()
{
	material = new Material(glm::vec3(LIGHT_AMBIENT), glm::vec3(LIGHT_DIFFUSE), glm::vec3(LIGHT_SPECULAR));
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
	std::vector<Vertex> spikeObj = LoadOBJ("OBJ/spikes.obj");
	std::vector<Vertex> endObj = LoadOBJ("OBJ/end.obj");
	std::vector<Vertex> winObj = LoadOBJ("OBJ/win.obj");

	win = new Model(glm::vec3(0, 0, 0), material, groundDif, groundSpec, winObj);
	win->SetEnable(false);

	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (mapGrounds[j][i] == MAP_GROUND)
			{
				groundObjects.push_back(new Model(glm::vec3(i*2.f, -0.5f, j * 2.f), material, groundDif, groundSpec, cubeObj));
			}

			if (mapObjects[j][i] == MAP_BOUND)
			{
				boundsObjects.push_back(new GameObject(new Model(glm::vec3(i*2.f, 0.5f, j * 2.f), nullptr, nullptr, nullptr, std::vector<Vertex>(), 0.5f)));
			}
			if (mapObjects[j][i] == MAP_START)
			{
				startPosition = glm::vec3(i*2.f, 1, j * 2.f);
				player = new Player(new Model(startPosition, material, playerDif, playerSpec, playerObj, 1));
				camera = new Camera(glm::vec3(i*2.f, CAMERA_Y, j * 2.f));
			}
			if (mapObjects[j][i] == MAP_TREE)
			{
				gameObjects.push_back(new GameObject(new Model(glm::vec3(i*2.f, 0.5f, j * 2.f), material, treeTextureDif, treeTextureSpec, treeObj, 0.5f)));
			}
			if (mapObjects[j][i] == MAP_OBSTACLE)
			{
				obstacleObjects.push_back(new Obstacle(new Model(glm::vec3(i*2.f, 0.5f, j * 2.f), material, treeTextureDif, treeTextureSpec, spikeObj, 1.0f)));
			}
			if (mapObjects[j][i] == MAP_END)
			{
				lightPosition = glm::vec3(i*2.f, 5.0f, j * 2.f);
				end = new GameObject(new Model(glm::vec3(i*2.f, 0.5f, j * 2.f), material, treeTextureDif, treeTextureSpec, endObj, 0.5f));
			}
		}
	}
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
	float timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (timeSinceStart - time) / 1000;
	time = timeSinceStart;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Bind();
	camera->SetUniforms(shader);
	pointLights[0]->SetUniforms(shader);

	if (player->SphereRectCollision(end->GetModel()))
	{
		Win();
	}

	for (int i = 0; i < boundsObjects.size(); i++)
	{
		if (player->SphereRectCollision(boundsObjects[i]->GetModel()))
		{
			PlayerMove(-playerMoveDirection);
		}
	}


	for (int i = 0; i < obstacleObjects.size(); i++)
	{
		if (player->SphereRectCollision(obstacleObjects[i]->GetModel()))
		{
			RestartPlayerPosition();
			return;
		}

		obstacleObjects[i]->Render(shader);
	}

	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (player->SphereRectCollision(gameObjects[i]->GetModel()))
		{
			PlayerMove(-playerMoveDirection);
			return;
		}

		gameObjects[i]->Render(shader);
	}

	for (int i = 0; i < groundObjects.size(); i++)
	{
		groundObjects[i]->Render(shader);
	}


	player->Render(shader);
	end->Render(shader);
	win->Render(shader);
	
	glFlush();
	glutSwapBuffers();
}

void Win()
{
	std::cout << "wygranko" << std::endl;
	camera->SetStartAngles();
	camera->SetEnable(false);
	win->SetEnable(true);
	glutPostRedisplay();
}

void RestartPlayerPosition()
{
	player->GetModel()->SetPosition(startPosition);
	camera->SetPosition(glm::vec3(startPosition.x, CAMERA_Y, startPosition.z));
	camera->SetStartAngles();
	camera->SetEnable(true);
	win->SetEnable(false);
}

void PlayerMove(glm::vec3 cameraDirection)
{
	//playerMoveDirection = normalize(glm::vec3(cameraDirection.x, 0, cameraDirection.z)) * deltaTime;
	playerMoveDirection = normalize(glm::vec3(cameraDirection.x, 0, cameraDirection.z));
	camera->Move(playerMoveDirection);
	player->Move(playerMoveDirection);
	win->SetPosition(camera->GetPosition() + glm::vec3(-1, -2, 0));
}

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
		camera->Move(camera->GetRight());
	else if (key == GLUT_KEY_LEFT)
		camera->Move(-camera->GetRight());
	else if (key == GLUT_KEY_UP)
		camera->Move(camera->GetFront());
	else if (key == GLUT_KEY_DOWN)
		camera->Move(-camera->GetFront());

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
		PlayerMove(camera->GetFront());
	}
	if (key == 's')
	{
		PlayerMove(-camera->GetFront());
	}
	if (key == 'a')
	{
		PlayerMove(-camera->GetRight());
	}
	if (key == 'd')
	{
		PlayerMove(camera->GetRight());
	}
	if (key == 'r')
	{
		RestartPlayerPosition();
	}

	glutPostRedisplay();
}
// ----------------------------------------------------------
// MouseButtonHandler() Callback Function
// ----------------------------------------------------------
void MouseButtonHandler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		firstMouse = true;
	}
}
// ----------------------------------------------------------
// MouseMoveHandler() Callback Function
// ----------------------------------------------------------
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

	camera->ProcessMouseMovement(xoffset, yoffset);
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
	delete player;
	delete end;
	delete camera;
	return 0;
}