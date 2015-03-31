#ifndef _LIGHTING_
#define _LIGHTING_

#include "glm/glm.hpp"
#include <OpenGL/gl3.h>
#include <vector>

typedef	enum 
{
	LIGHT_SOURCE_DIRECTION_TYPE_DIRECTIONAL = 1,
	LIGHT_SOURCE_DIRECTION_TYPE_POSITIONAL = 0
} LIGHT_SOURCE_DIRECTION_TYPE;

class LightSource
{
	public:
		LightSource(glm::vec3 color, glm::vec3 locationOrDirection, GLfloat specularExponent, LIGHT_SOURCE_DIRECTION_TYPE directionType) :
			_color(color), _locationOrDirection(locationOrDirection), _specularExponent(specularExponent), _directionType(directionType) {}
		~LightSource() = default;

		glm::vec3 color() { return _color; }
		glm::vec3 locationOrDirection() { return _locationOrDirection; }
		GLfloat specularExponent() { return _specularExponent; }
		LIGHT_SOURCE_DIRECTION_TYPE isDirectional() { return _directionType; }

	private:
		glm::vec3 _color;
		glm::vec3 _locationOrDirection;
		GLfloat _specularExponent;
		LIGHT_SOURCE_DIRECTION_TYPE _directionType;

};

class LightSourceLoader
{
	public:
		LightSourceLoader() = default;
		~LightSourceLoader();

		void addLightSource(LightSource* lightSource);
		void load(GLuint shaderProgram);

	private:
		std::vector<LightSource*> _lightSources;
};

#endif