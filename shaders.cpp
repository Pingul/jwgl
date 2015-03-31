#include "misc.hpp"
#include "shaders.hpp"
#include "GL_utilities.hpp"
#include "LoadTGA.hpp"

static char TEXTURE_UNIT_STRING[] = "TEXTURE_UNIT_-";

Texture::Texture(const char* filePath)
{
	LoadTGATextureSimple((char*)filePath, &this->_textureID);
}

void Texture::activate(int textureUnit, GLuint shaderProgram)
{
	switch (textureUnit)
	{
		case 0:
		glActiveTexture(GL_TEXTURE0);
			break;
		case 1:
		glActiveTexture(GL_TEXTURE1);
			break;
		case 2:
		glActiveTexture(GL_TEXTURE2);
			break;
		case 3:
		glActiveTexture(GL_TEXTURE3);
			break;
		case 4:
		glActiveTexture(GL_TEXTURE4);
			break;
	}
	TEXTURE_UNIT_STRING[13] = (char)textureUnit + 48; // ugly, but needed to make it into ASCII
	glBindTexture(GL_TEXTURE_2D, this->_textureID);
	glUniform1i(glGetUniformLocation(shaderProgram, TEXTURE_UNIT_STRING), textureUnit);
}


Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
	this->_programID = loadShaders(vertexShader, fragmentShader);
}

void Shader::use()
{
	glUseProgram(this->_programID);
	int index = 0;
	for (auto &it : this->_textures)
	{
		it->activate(index, this->_programID);
		++index;
	}
}

GLuint Shader::ID()
{
	return this->_programID;
}

void Shader::addTexture(Texture* texture)
{
	this->_textures.push_back(texture);
}

Shader::~Shader()
{
	for (auto &it : this->_textures)
	{	
		delete it;
	}
}


void ShaderManager::add(const char* name, Shader* shader)
{
	auto iterator = this->_shaders.find(name);
	if (iterator != this->_shaders.end())
		throw std::logic_error("Shader with that name already exists.");

	std::string programName(name);
	this->_shaders.insert(std::pair<std::string, Shader*>(programName, shader));
	this->use(name);
	// this->_inUse = shader;
}

void ShaderManager::use(const char* name)
{
	auto iterator = this->_shaders.find(name);
	if (iterator == this->_shaders.end())
		throw std::logic_error("No shader with that name exists.");

	iterator->second->use();
	this->_inUse = iterator->second;
}

Shader* ShaderManager::get()
{
	if (this->_shaders.empty())
		throw std::logic_error("No shaders exists.");

	return this->_inUse;
}

Shader* ShaderManager::get(const char* name)
{
	auto iterator = this->_shaders.find(name);
	if (iterator == this->_shaders.end())
		throw std::logic_error("No shader with that name exists.");

	return iterator->second;
}

ShaderManager::~ShaderManager()
{
	for (auto &it : this->_shaders)
	{
		delete it.second;
	}
}