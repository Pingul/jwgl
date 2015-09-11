#include <OpenGL/gl3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "misc.hpp"
#include "WindowHandler.hpp"
// #include "ProgramGraphics.hpp"
#include "GraphicsHandler.hpp"

class Graphics: public jwgl::GraphicsHandler
{
	public:
		Graphics() = default;
		virtual ~Graphics();

		virtual void init();
		virtual void drawFrame(float t);
};

void Graphics::init()
{
	std::cout << "init" << std::endl;

	
}

void Graphics::drawFrame(float t)
{

}

Graphics::~Graphics() {}





int main()
{
	try
	{
		jwgl::WindowHandler handler(new Graphics);
		handler.createWindow();
		handler.displayWindow();
	} 
	catch (std::exception& error)
	{
		std::cout << "ERROR: " << error.what() << std::endl;
		std::cout << "abort" << std::endl;
	}

	return 0;
}
