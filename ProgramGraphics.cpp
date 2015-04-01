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

#define NEAR 1.0
#define FAR 300.0
#define RIGHT 1.0
#define LEFT -1.0
#define BOTTOM -1.0
#define TOP 1.0

void ProgramGraphics::init()
{
	printError("--"); // This function seems to generate one extra error in the beginning, don't know why

	setupOpenGL();
	loadShaders();
	loadLightSources();
	loadModels();
	setupCamera();
}

void ProgramGraphics::setupOpenGL()
{
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	printError("init opengl");
}

void ProgramGraphics::loadShaders()
{
	this->_shaders = new ShaderManager;
	Shader* shader = new Shader("shaders/light_texture.vert", "shaders/light_texture.frag");
	Texture* texture1 = new Texture("textures/maskros512.tga");
	Texture* texture2 = new Texture("textures/ground_1024_Q3.tga");
	shader->addTexture(texture1);
	shader->addTexture(texture2);
	this->_shaders->add("test", shader);

	printError("init shader");
}

void ProgramGraphics::loadLightSources()
{
	this->_lightSources = new LightSourceLoader();
	LightSource* light1 = new LightSource(glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 5.0), 50.0, LIGHT_SOURCE_DIRECTION_TYPE_POSITIONAL);
	LightSource* light2 = new LightSource(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -5.0), 150.0, LIGHT_SOURCE_DIRECTION_TYPE_POSITIONAL);
	this->_lightSources->addLightSource(light1);
	this->_lightSources->addLightSource(light2);
	this->_lightSources->load(this->_shaders->get()->ID());

	printError("upload light sources");
}

void ProgramGraphics::loadModels()
{
	Bunny* bunny1 = new Bunny;
	Bunny* bunny2 = new Bunny;
	Bunny* bunny3 = new Bunny;
	Bunny* bunny4 = new Bunny;
	bunny1->move(glm::vec3(1.0, 0.0, 0.0));
	bunny2->move(glm::vec3(-1.0, 0.0, 0.0));
	bunny3->move(glm::vec3(0.0, 1.0, 0.0));
	bunny4->move(glm::vec3(0.0, -1.0, 0.0));
	this->_worldObjects.push_back(bunny1);
	this->_worldObjects.push_back(bunny2);
	this->_worldObjects.push_back(bunny3);
	this->_worldObjects.push_back(bunny4);

	Terrain* terrain = Terrain::generate("models/fft-terrain.tga");
	this->_worldObjects.push_back(terrain);	

	glm::mat4 MTW = glm::mat4();
	glm::mat4 WTV = glm::mat4();
	glm::mat4 projectionMatrix = glm::frustum(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
	glUniformMatrix4fv(glGetUniformLocation(this->_shaders->get()->ID(), "MTW"), 1, GL_FALSE, glm::value_ptr(MTW));
	glUniformMatrix4fv(glGetUniformLocation(this->_shaders->get()->ID(), "WTV"), 1, GL_FALSE, glm::value_ptr(WTV));
	glUniformMatrix4fv(glGetUniformLocation(this->_shaders->get()->ID(), "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	printError("upload data");
}

void ProgramGraphics::setupCamera()
{
	glm::vec3 location(0, 0, 15);
	glm::vec3 lookingAt(0, 0, 0);
	glm::vec3 upDirection(0, 1, 0);
	this->_camera = new Camera(location, lookingAt, upDirection);
}

void ProgramGraphics::drawFrame(float t)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->handleKeys();
	this->handleMouseMovement();

	glm::mat4 WTV = this->_camera->WTVMatrix();
	glUniformMatrix4fv(glGetUniformLocation(this->_shaders->get()->ID(), "WTV"), 1, GL_FALSE, glm::value_ptr(WTV));

	for (auto it = this->_worldObjects.begin(); it != this->_worldObjects.end(); ++it)
	{
		(*it)->draw(this->_shaders->get()->ID());	
	}
	
	printError("display");
}

void ProgramGraphics::handleKeys()
{
	static glm::vec3 step(0, 0, 0);
	char stepTaken = 0;

	if (glfwGetKey(this->_window, GLFW_KEY_W))
	{
		stepTaken = 1;
		step.z = 1;
	} 
	else if (glfwGetKey(this->_window, GLFW_KEY_S))
	{
		stepTaken = 1;
		step.z = -1;
	}
	
	if (glfwGetKey(this->_window, GLFW_KEY_D))
	{
		stepTaken = 1;
		step.x = 1;
	} 
	else if (glfwGetKey(this->_window, GLFW_KEY_A))
	{
		stepTaken = 1;
		step.x = -1;
	}

	if (glfwGetKey(this->_window, GLFW_KEY_SPACE))
	{
		stepTaken = 1;
		step.y = 1;
	} 
	else if (glfwGetKey(this->_window, GLFW_KEY_C))
	{
		stepTaken = 1;
		step.y = -1;
	}

	if (stepTaken)
	{
		this->_camera->takeStep(step);
		stepTaken = 0;
		step.x = 0;
		step.y = 0;
		step.z = 0;
	}
}

void ProgramGraphics::handleMouseMovement()
{
	double xPos;
	double yPos;
	glfwGetCursorPos(this->_window, &xPos, &yPos);
	this->_camera->reorient((float)xPos, (float)yPos);
}

 ProgramGraphics::~ProgramGraphics()
 {
 	delete this->_camera;
 	delete this->_shaders;
 	delete this->_lightSources;

 	for (auto &it : this->_worldObjects)
 	{
 		delete it;
 	}
 }