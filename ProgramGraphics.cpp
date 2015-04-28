#include <OpenGL/gl3.h>
#include "misc.hpp"
#include "GL_utilities.hpp"
#include "loadobj.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ProgramGraphics.hpp"
#include "shaders.hpp"
#include "models.hpp"
#include "camera.hpp"
#include "lighting.hpp"
#include "terrain.hpp"
#include "physics.hpp"

#define NEAR 1.0
#define FAR 300.0
#define RIGHT 1.0
#define LEFT -1.0
#define BOTTOM -1.0
#define TOP 1.0

void handleSphereObjectMovement();
void handleCameraMovement();

void ProgramGraphics::init()
{
	printError("--"); // This function seems to generate one extra error in the beginning, don't know why

	setupOpenGL();
	loadShaders();
	loadLightSources();
	loadModels();
	std::cout << "camera" << std::endl;
	setupCamera();
	std::cout << "physics" << std::endl;
	setupPhysics();

	std::cout << "initialization complete" << std::endl;
}

void ProgramGraphics::setupOpenGL()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);

	printError("init opengl");
}

void ProgramGraphics::loadShaders()
{
	_shaders = new ShaderManager;
	Shader* shader = new Shader("shaders/light_texture.vert", "shaders/light_texture.frag");
	Texture* texture1 = new Texture("textures/maskros512.tga");
	Texture* texture2 = new Texture("textures/ground_1024_Q3.tga");
	shader->addTexture(texture1);
	shader->addTexture(texture2);
	_shaders->add("test", shader);

	printError("init shader");
}

void ProgramGraphics::loadLightSources()
{
	_lightSources = new LightSourceLoader();
	LightSource* light1 = new LightSource(glm::vec3(1.0, 0.0, 0.0), glm::vec3(20.0, 20.0, 20.0), 50.0, LIGHT_SOURCE_DIRECTION_TYPE_POSITIONAL);
	LightSource* light2 = new LightSource(glm::vec3(0.0, 1.0, 0.0), glm::vec3(20.0, 20.0, 20.0), 150.0, LIGHT_SOURCE_DIRECTION_TYPE_POSITIONAL);
	// LightSource* light3 = new LightSource(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.1, -0.8, 0.1), 2.0, LIGHT_SOURCE_DIRECTION_TYPE_DIRECTIONAL);
	_lightSources->addLightSource(light1);
	_lightSources->addLightSource(light2);
	// _lightSources->addLightSource(light3);
	_lightSources->load(_shaders->get()->ID());

	printError("upload light sources");
}


void ProgramGraphics::loadModels()
{
	_objectManager = new WorldObjectManager();
	// Bunny* bunny = new Bunny;
	// Sphere* sphere = new Sphere;
	// sphere->move(glm::vec3(20.0, 10.0, 20.0));
	// _objectManager->registerWorldObject(sphere);

	_terrainGenerator = new TerrainGenerator;
	Terrain* terrain = _terrainGenerator->generateTerrain(16, 16);
	std::cout << "generation complete" << std::endl;
	_objectManager->registerWorldObject(terrain);

	Sphere* sphere = new Sphere;
	sphere->move(glm::vec3(100.0, 3, 100.0));
	_objectManager->registerWorldObject(sphere);

	glm::mat4 MTW = glm::mat4();
	glm::mat4 WTV = glm::mat4();
	glm::mat4 projectionMatrix = glm::frustum(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
	glUniformMatrix4fv(glGetUniformLocation(_shaders->get()->ID(), "MTW"), 1, GL_FALSE, glm::value_ptr(MTW));
	glUniformMatrix4fv(glGetUniformLocation(_shaders->get()->ID(), "WTV"), 1, GL_FALSE, glm::value_ptr(WTV));
	glUniformMatrix4fv(glGetUniformLocation(_shaders->get()->ID(), "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	std::cout << "upload complete" << std::endl;
	printError("upload data");
}

void ProgramGraphics::setupCamera()
{
	glm::vec3 lookingAt = glm::vec3(0, 0, 0);//_objectManager->objects()->at(0)->at();
	glm::vec3 location = lookingAt - glm::vec3(10, -2, 10);
	glm::vec3 upDirection(0, 1, 0);
	_camera = new Camera(location, lookingAt, upDirection);
}

void ProgramGraphics::setupPhysics()
{
	_physics = new Physics;
	_physics->registerObjectManager(_objectManager);
}

bool calculatePositions = true;

void ProgramGraphics::drawFrame(float t)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	handleKeys();
	handleMouseMovement();
	// if (calculatePositions)
	// {
		_physics->calculatePositions(t);
		calculatePositions = false;
	// }

	glm::mat4 WTV = _camera->WTVMatrix();
	glUniformMatrix4fv(glGetUniformLocation(_shaders->get()->ID(), "WTV"), 1, GL_FALSE, glm::value_ptr(WTV));

	for (auto it = _objectManager->terrain()->begin(); it != _objectManager->terrain()->end(); ++it)
	{
		(*it)->draw(_shaders->get()->ID());	
	}	

	for (auto it = _objectManager->objects()->begin(); it != _objectManager->objects()->end(); ++it)
	{
		(*it)->draw(_shaders->get()->ID());	
	}
	
	printError("display");
}

void ProgramGraphics::handleKeys()
{
	handleCameraMovement();
	handleSphereObjectMovement();
}

void handleCameraMovement()
{
	static glm::vec3 step(0, 0, 0);
	char stepTaken = 0;

	if (glfwGetKey(_window, GLFW_KEY_W))
	{
		stepTaken = 1;
		step.z = 1;
	} 
	else if (glfwGetKey(_window, GLFW_KEY_S))
	{
		stepTaken = 1;
		step.z = -1;
	}
	
	if (glfwGetKey(_window, GLFW_KEY_D))
	{
		stepTaken = 1;
		step.x = 1;
	} 
	else if (glfwGetKey(_window, GLFW_KEY_A))
	{
		stepTaken = 1;
		step.x = -1;
	}

	if (glfwGetKey(_window, GLFW_KEY_SPACE))
	{
		stepTaken = 1;
		step.y = 1;
	} 
	else if (glfwGetKey(_window, GLFW_KEY_C))
	{
		stepTaken = 1;
		step.y = -1;
	}


	if (stepTaken)
	{
		_camera->takeStep(step);
		stepTaken = 0;
		step.x = 0;
		step.y = 0;
		step.z = 0;
	}
}

void handleSphereObjectMovement()
{
	Sphere* sphere = (Sphere*)_objectManager->objects()->at(0);
	float sphereStep = 0.1;
	if (glfwGetKey(_window, GLFW_KEY_L))
	{
		sphere->move(sphere->at() + glm::vec3(0, -sphereStep, 0));
		calculatePositions = true;
	}
	else if (glfwGetKey(_window, GLFW_KEY_O))
	{
		sphere->move(sphere->at() + glm::vec3(0, sphereStep, 0));
		calculatePositions = true;
	}
	else if (glfwGetKey(_window, GLFW_KEY_UP))
	{
		sphere->move(sphere->at() + glm::vec3(0, 0, -sphereStep));
		calculatePositions = true;
	} 
	else if (glfwGetKey(_window, GLFW_KEY_DOWN))
	{
		sphere->move(sphere->at() + glm::vec3(0, 0, sphereStep));
		calculatePositions = true;
	} 
	else if (glfwGetKey(_window, GLFW_KEY_LEFT))
	{
		sphere->move(sphere->at() + glm::vec3(-sphereStep, 0, 0));
		calculatePositions = true;
	}
	else if (glfwGetKey(_window, GLFW_KEY_RIGHT))
	{
		sphere->move(sphere->at() + glm::vec3(sphereStep, 0, 0));
		calculatePositions = true;
	}
}


void ProgramGraphics::handleMouseMovement()
{
	double xPos;
	double yPos;
	glfwGetCursorPos(_window, &xPos, &yPos);
	_camera->reorient((float)xPos, (float)yPos);
}

 ProgramGraphics::~ProgramGraphics()
 {
 	delete _camera;
 	delete _shaders;
 	delete _lightSources;
 	delete _objectManager;
 	delete _physics;
 	delete _terrainGenerator;	
 }