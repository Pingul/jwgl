#ifndef _MODELS_
#define _MODELS_

#include <map>
#include <stdlib.h>
#include <OpenGL/gl3.h>
#include "loadobj.hpp"
#include <glm/glm.hpp>

typedef enum
{
	MODEL_TYPE_BUNNY,
} MODEL_TYPE;

// At the moment only using this as a wrapper class for loadobj.hpp typedef.
class VertexModel
{
	public:
		VertexModel(Model* model) :
			_tModel(model) {}
		~VertexModel() { free(this->_tModel); }

		void draw(GLuint shaderProgram, const char* inPosition, const char* inNormal, const char* inTexture);

	private:
		Model* _tModel;
};

class ModelManager
{
	public:
		ModelManager() = default;
		~ModelManager();

		VertexModel* load(MODEL_TYPE);
		VertexModel* getReference(MODEL_TYPE);
		bool isLoaded(MODEL_TYPE);

		static ModelManager* shared();

	private:
		std::map<MODEL_TYPE, VertexModel*> _models;

		static ModelManager* _shared;

};

class WorldObject
{
	public:
		WorldObject() = default;
		virtual ~WorldObject() = default;

		virtual void draw(GLuint shaderProgram) = 0;
		virtual void move(glm::vec3 to) { this->_location = to; };
		virtual glm::vec3 at() { return _location; }

	protected:
		glm::vec3 _location;
		VertexModel* _model;
};

class Bunny : public WorldObject 
{
	public:
		Bunny();
		~Bunny() = default;

		virtual void draw(GLuint shaderProgram);
};


#endif