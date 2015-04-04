#include "misc.hpp"

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "models.hpp"

void VertexModel::loadModel(const char* filePath)
{
	_tModel = LoadModelPlus(filePath);

	_vertexArray = _tModel->vertexArray;
	_normalArray = _tModel->normalArray;
	_texCoordArray = _tModel->texCoordArray;
	_indexArray = _tModel->indexArray;
}

void VertexModel::loadModel(Model* model)
{
	_tModel = model;

	_vertexArray = _tModel->vertexArray;
	_normalArray = _tModel->normalArray;
	_texCoordArray = _tModel->texCoordArray;
	_indexArray = _tModel->indexArray;
}

void VertexModel::drawModel(GLuint shaderProgram, const char* inPosition, const char* inNormal, const char* inTexture)
{
	DrawModel(_tModel, shaderProgram, inPosition, inNormal, inTexture);
}


ModelManager* ModelManager::_shared = new ModelManager;

ModelManager* ModelManager::shared()
{
	return ModelManager::_shared;
}

ModelManager::~ModelManager()
{
	for (auto it = _models.begin(); it != _models.end(); ++it)
	{
		free(it->second);
	}
}

VertexModel* ModelManager::load(MODEL_TYPE type)
{
	VertexModel* vertexModel = new VertexModel;
	switch (type)
	{
		case MODEL_TYPE_BUNNY:
		vertexModel->loadModel("models/bunnyplus.obj");
			break;
		case MODEL_TYPE_SPHERE:
		vertexModel->loadModel("models/sphere.obj");
			break;
		default:
			throw std::logic_error("load: Model not loaded");
	}

	_models.insert(std::pair<MODEL_TYPE, VertexModel*>(type, vertexModel));
	return vertexModel;
}


VertexModel* ModelManager::getReference(MODEL_TYPE type)
{
	auto it = _models.find(type);
	if (it == _models.end())
		throw std::logic_error("getReference: Model not loaded");

	return it->second;
}

bool ModelManager::isLoaded(MODEL_TYPE type)
{
	return _models.find(type) != _models.end();
}

void WorldObject::loadMTWMatrixToGPU(GLuint shaderProgram)
{
	glm::mat4 MTW = glm::translate(glm::mat4(1.0), _location);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MTW"), 1, GL_FALSE, glm::value_ptr(MTW));	
}




// ----- MODELS

Bunny::Bunny()
{
	ModelManager* models = ModelManager::shared();
	if (!models->isLoaded(MODEL_TYPE_BUNNY))
		_model = models->load(MODEL_TYPE_BUNNY);
	else
		_model = models->getReference(MODEL_TYPE_BUNNY);
}

void Bunny::draw(GLuint shaderProgram)
{
	loadMTWMatrixToGPU(shaderProgram);
	_model->drawModel(shaderProgram, "in_Position", "in_Normal", "in_TextureCoordinates");
}

Sphere::Sphere()
{
	ModelManager* models = ModelManager::shared();
	if (!models->isLoaded(MODEL_TYPE_SPHERE))
		_model = models->load(MODEL_TYPE_SPHERE);
	else
		_model = models->getReference(MODEL_TYPE_SPHERE);
}

void Sphere::draw(GLuint shaderProgram)
{
	loadMTWMatrixToGPU(shaderProgram);
	_model->drawModel(shaderProgram, "in_Position", "in_Normal", "in_TextureCoordinates");
}
