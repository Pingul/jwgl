#include "misc.hpp"

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "models.hpp"

void VertexModel::draw(GLuint shaderProgram, const char* inPosition, const char* inNormal, const char* inTexture)
{
	DrawModel(this->_tModel, shaderProgram, inPosition, inNormal, inTexture);
}

ModelManager* ModelManager::_shared = new ModelManager;

ModelManager* ModelManager::shared()
{
	return ModelManager::_shared;
}

ModelManager::~ModelManager()
{
	for (auto it = this->_models.begin(); it != this->_models.end(); ++it)
	{
		free(it->second);
	}
}

VertexModel* ModelManager::load(MODEL_TYPE type)
{
	Model* model;
	switch (type)
	{
		case MODEL_TYPE_BUNNY:
		model = LoadModelPlus("models/bunnyplus.obj");
			break;
		default:
			throw std::logic_error("load: Model not loaded");
	}

	VertexModel* vertexModel = new VertexModel(model);
	this->_models.insert(std::pair<MODEL_TYPE, VertexModel*>(type, vertexModel));
	return vertexModel;
}


VertexModel* ModelManager::getReference(MODEL_TYPE type)
{
	auto it = this->_models.find(type);
	if (it == this->_models.end())
		throw std::logic_error("getReference: Model not loaded");

	return it->second;
}

bool ModelManager::isLoaded(MODEL_TYPE type)
{
	return this->_models.find(type) != this->_models.end();
}

void WorldObject::loadMTWMatrixToGPU(GLuint shaderProgram)
{
	glm::mat4 MTW = glm::translate(glm::mat4(1.0), this->_location);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MTW"), 1, GL_FALSE, glm::value_ptr(MTW));	
}




// ----- MODELS

Bunny::Bunny()
{
	ModelManager* models = ModelManager::shared();
	if (!models->isLoaded(MODEL_TYPE_BUNNY))
		this->_model = models->load(MODEL_TYPE_BUNNY);
	else
		this->_model = models->getReference(MODEL_TYPE_BUNNY);
}

void Bunny::draw(GLuint shaderProgram)
{
	loadMTWMatrixToGPU(shaderProgram);
	this->_model->draw(shaderProgram, "in_Position", "in_Normal", "in_TextureCoordinates");
}
