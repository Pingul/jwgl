#include "misc.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VisualizationGraphicsHandler.hpp"
#include <iostream>
#include "camera.hpp"
#include "shaders.hpp"
#include "lighting.hpp"
#include "GL_utilities.hpp"
#include "simulation.hpp"
#include "models.hpp"

#define NEAR 1.0
#define FAR 300.0
#define RIGHT 1.0
#define LEFT -1.0
#define BOTTOM -1.0
#define TOP 1.0

void VisualizationGraphicsHandler::init()
{
	loadShaders();
	loadLightSources();
	setupCamera();
	setupOpenGL();

	_simulation = new Simulation{"sim-format.txt"};
	_worldObjects = new WorldObjectManager{};
	for (int i = 0; i < _simulation->nbrObjects(); i++)
	{
		_worldObjects->registerWorldObject(new Sphere());
	}
}

void VisualizationGraphicsHandler::setupOpenGL()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 WTV = glm::mat4();
	glm::mat4 projectionMatrix = glm::frustum(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
	glUniformMatrix4fv(glGetUniformLocation(_shaders->get()->ID(), "WTV"), 1, GL_FALSE, glm::value_ptr(WTV));
	glUniformMatrix4fv(glGetUniformLocation(_shaders->get()->ID(), "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	printError("init opengl");
}

void VisualizationGraphicsHandler::loadShaders()
{
	_shaders = new ShaderManager;
	Shader* shader = new Shader("shaders/light_texture.vert", "shaders/light_texture.frag");
	Texture* texture = new Texture("textures/ground_1024_Q3.tga");
	shader->addTexture(texture);
	_shaders->add("test", shader);

	printError("init shader");
}

void VisualizationGraphicsHandler::loadLightSources()
{
	_lightSources = new LightSourceLoader();
	LightSource* light1 = new LightSource(glm::vec3(1.0, 0.0, 0.0), glm::vec3(20.0, 20.0, 20.0), 50.0, LIGHT_SOURCE_DIRECTION_TYPE_POSITIONAL);
	LightSource* light2 = new LightSource(glm::vec3(0.0, 1.0, 0.0), glm::vec3(20.0, 20.0, 20.0), 150.0, LIGHT_SOURCE_DIRECTION_TYPE_POSITIONAL);
	_lightSources->addLightSource(light1);
	_lightSources->addLightSource(light2);
	_lightSources->load(_shaders->get()->ID());
	printError("upload light sources");
}


void VisualizationGraphicsHandler::setupCamera()
{
	glm::vec3 lookingAt = glm::vec3(0, 0, 0);
	glm::vec3 location = glm::vec3(5, 5, 0);
	glm::vec3 upDirection(0, 1, 0);
	_camera = new Camera(location, lookingAt, upDirection);
}

void VisualizationGraphicsHandler::drawFrame(float t)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_simulation->updatePositions(*_worldObjects->objects(), t);

	glm::mat4 WTV = _camera->WTVMatrix();
	glUniformMatrix4fv(glGetUniformLocation(_shaders->get()->ID(), "WTV"), 1, GL_FALSE, glm::value_ptr(WTV));

	// static int i = 0;
	// if (i == 0)
	// {
	// 	i++;
	// 	std::cout << "size: " << _worldObjects->objects()->size() << std::endl;
	// 	for (const auto& obj : *_worldObjects->objects())
	// 	{
	// 		std::cout << "(" << obj->at().x << ", " << obj->at().y << ", " << obj->at().z << ")" << std::endl;
	// 	}
	// }

	for (const auto& obj : *_worldObjects->objects())
	{
		obj->draw(_shaders->get()->ID());
	}
}


VisualizationGraphicsHandler::~VisualizationGraphicsHandler()
{
	delete _shaders;
	delete _camera;
	delete _lightSources;
	delete _simulation;
	delete _worldObjects;
}