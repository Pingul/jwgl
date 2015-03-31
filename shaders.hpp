#ifndef _SHADERS_
#define _SHADERS_

#include <OpenGL/gl3.h>
#include <map>
#include <string>
#include <vector>

// Make sure the GPU shaders are set up as correctly to accomodate for
// the textures added to a shader program.
class Texture
{
	public:
		Texture(const char* filePath);
		~Texture() = default;

		void activate(int textureUnit, GLuint shaderProgram);

	private:
		GLuint _textureID;
};

class Shader
{
	public:
		Shader(const char* vertexShader, const char* fragmentShader);
		~Shader();

		void use();
		GLuint ID();

		// Textures are added to the back, meaning that, in the GPU shaders, you access the textures by
		//	TEXTURE_UNIT_0 : first texture
		//	TEXTURE_UNIT_1 : second texture
		//	etc
		void addTexture(Texture* texture);

	private:
		GLuint _programID;
		std::vector<Texture*> _textures;
};

class ShaderManager
{
	public:
		ShaderManager() = default;
		~ShaderManager();

		void add(const char* name, Shader* shader);
		void use(const char* name);
		Shader* get(); // Returns current shader
		Shader* get(const char* name);

	private:
		std::map<std::string, Shader*> _shaders;
		Shader* _inUse;
};

#endif