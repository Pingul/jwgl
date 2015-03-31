#include "misc.hpp"
#include "lighting.hpp"
#include <glm/gtc/type_ptr.hpp>

void LightSourceLoader::addLightSource(LightSource* lightSource)
{
	this->_lightSources.push_back(lightSource);
}

void LightSourceLoader::load(GLuint shaderProgram)
{
	int lightSourceCount = this->_lightSources.size();

	glm::vec3* lightSourceColor = (glm::vec3*)malloc(lightSourceCount*sizeof(glm::vec3));
	glm::vec3* lightSourceDirectionOrPosition = (glm::vec3*)malloc(lightSourceCount*sizeof(glm::vec3));
	GLfloat* specularExponent = (GLfloat*)malloc(lightSourceCount*sizeof(GLfloat));
	GLint* isDirectional = (GLint*)malloc(lightSourceCount*sizeof(GLint));

	int index = 0;
	for (const auto &i : this->_lightSources)
	{
		lightSourceColor[index] = i->color();
		lightSourceDirectionOrPosition[index] = i->locationOrDirection();
		specularExponent[index] = i->specularExponent();
		isDirectional[index] = i->isDirectional();
		++index;
	}

	glUniform3fv(glGetUniformLocation(shaderProgram, "lightSourceDirectionOrPosition"), 4, glm::value_ptr(lightSourceDirectionOrPosition[0]));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightSourceColor"), 4, glm::value_ptr(lightSourceColor[0]));
	glUniform1fv(glGetUniformLocation(shaderProgram, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(shaderProgram, "isDirectional"), 4, isDirectional);

	free(lightSourceColor);
	free(lightSourceDirectionOrPosition);
	free(specularExponent);
	free(isDirectional);
}

LightSourceLoader::~LightSourceLoader()
{
	for (auto &it : this->_lightSources)
	{
		delete it;
	}
}