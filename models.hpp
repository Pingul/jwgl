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
	MODEL_TYPE_SPHERE,
} MODEL_TYPE;

// At the moment only using this as a wrapper class for loadobj.hpp typedef.
class VertexModel
{
	public:
		VertexModel() = default;
		~VertexModel() { free(_tModel); }

		void loadModel(const char* filePath);
		void loadModel(Model* model); // In cases where the model is already create (e.g. terrain)
		void drawModel(GLuint shaderProgram, const char* inPosition, const char* inNormal, const char* inTexture);
		GLfloat* vertexArray() { return _vertexArray; }
		GLfloat* normalArray() { return _normalArray; }
		GLfloat* texCoordArray() { return _texCoordArray; }
		GLuint* indexArray() { return _indexArray; }

	private:
		GLfloat* _vertexArray;
		GLfloat* _normalArray;
		GLfloat* _texCoordArray;
		GLuint* _indexArray;

		Model* _tModel; // Avoid using, as we're trying to wrap its functionality

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
	friend class Physics;

	public:
		WorldObject() = default;
		virtual ~WorldObject() = default;

		virtual void draw(GLuint shaderProgram) = 0;
		virtual void move(glm::vec3 to) { _location = to; }
		virtual void accelerate(glm::vec3 to) { _velocity = to; }
		virtual glm::vec3 at() { return _location; }
		virtual glm::vec3 velocity() { return _velocity; }
		virtual bool isAffectedByGravity() { return true; }

	protected:
		WorldObject(VertexModel* model) :
			_model(model) {}

		glm::vec3 _location;
		glm::vec3 _velocity;
		VertexModel* _model;

		virtual void loadMTWMatrixToGPU(GLuint shaderProgram);
};

class Bunny : public WorldObject 
{
	public:
		Bunny();
		~Bunny() = default;

		virtual void draw(GLuint shaderProgram);
};

class Sphere : public WorldObject
{
	public:
		Sphere();
		~Sphere() = default;

		virtual void draw(GLuint shaderProgram);
};


#endif