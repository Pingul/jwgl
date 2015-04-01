#include "misc.hpp"
#include "shaders.hpp"
#include "GL_utilities.hpp"
#include "LoadTGA.hpp"

static char TEXTURE_UNIT_STRING[] = "TEXTURE_UNIT_-";

Texture::Texture(const char* filePath)
{
	LoadTGATextureSimple((char*)filePath, &_textureID);
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
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glUniform1i(glGetUniformLocation(shaderProgram, TEXTURE_UNIT_STRING), textureUnit);
}


Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
	_programID = loadShaders(vertexShader, fragmentShader);
}

void Shader::use()
{
	glUseProgram(_programID);
	int index = 0;
	for (auto &it : _textures)
	{
		it->activate(index, _programID);
		++index;
	}
}

GLuint Shader::ID()
{
	return _programID;
}

void Shader::addTexture(Texture* texture)
{
	_textures.push_back(texture);
}

Shader::~Shader()
{
	for (auto &it : _textures)
	{	
		delete it;
	}
}


void ShaderManager::add(const char* name, Shader* shader)
{
	auto iterator = _shaders.find(name);
	if (iterator != _shaders.end())
		throw std::logic_error("Shader with that name already exists.");

	std::string programName(name);
	_shaders.insert(std::pair<std::string, Shader*>(programName, shader));
	use(name);
	// _inUse = shader;
}

void ShaderManager::use(const char* name)
{
	auto iterator = _shaders.find(name);
	if (iterator == _shaders.end())
		throw std::logic_error("No shader with that name exists.");

	iterator->second->use();
	_inUse = iterator->second;
}

Shader* ShaderManager::get()
{
	if (_shaders.empty())
		throw std::logic_error("No shaders exists.");

	return _inUse;
}

Shader* ShaderManager::get(const char* name)
{
	auto iterator = _shaders.find(name);
	if (iterator == _shaders.end())
		throw std::logic_error("No shader with that name exists.");

	return iterator->second;
}

ShaderManager::~ShaderManager()
{
	for (auto &it : _shaders)
	{
		delete it.second;
	}
}