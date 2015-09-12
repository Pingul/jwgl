#include "misc.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VisualizationGraphicsHandler.hpp"
#include "camera.hpp"
#include "shaders.hpp"
#include "lighting.hpp"
#include "GL_utilities.hpp"
#include "simulation.hpp"


void VisualizationGraphicsHandler::init()
{
	setupOpenGL();
	loadShaders();
	loadLightSources();
	setupCamera();

	
	Simulation* sim = new Simulation("sim-format.txt");
}

void VisualizationGraphicsHandler::setupOpenGL()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
	glm::vec3 location = glm::vec3(40, 40, -20);
	glm::vec3 upDirection(0, 1, 0);
	_camera = new Camera(location, lookingAt, upDirection);
}

void VisualizationGraphicsHandler::drawFrame(float t)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 WTV = _camera->WTVMatrix();
	glUniformMatrix4fv(glGetUniformLocation(_shaders->get()->ID(), "WTV"), 1, GL_FALSE, glm::value_ptr(WTV));
}


VisualizationGraphicsHandler::~VisualizationGraphicsHandler()
{
	delete _shaders;
	delete _camera;
	delete _lightSources;
}