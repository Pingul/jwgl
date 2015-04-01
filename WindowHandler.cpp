#define GLEW_STATIC

#include <GL/glew.h>
#include "misc.hpp"
#include "WindowHandler.hpp"


namespace jwgl
{
	static const int WINDOW_WIDTH = 800;
	static const int WINDOW_HEIGHT = 600;

	WindowHandler::WindowHandler(GraphicsHandler* graphicsHandler)
	{
		_graphicsHandler = graphicsHandler;

		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}

	void WindowHandler::createWindow()
	{
		_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "jwproj", nullptr, nullptr);

		glfwMakeContextCurrent(_window);

		glewExperimental = GL_TRUE;
		glewInit();	

		_graphicsHandler->registerWindow(_window);
		_graphicsHandler->init();
	}

	void WindowHandler::displayWindow()
	{
		if (!_window)
			throw std::runtime_error("No window created.");

		while (!glfwWindowShouldClose(_window))
		{
			_graphicsHandler->drawFrame(0);

			glfwSwapBuffers(_window);
			glfwPollEvents();

			if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(_window, GL_TRUE);
			}
		}
	}

	WindowHandler::~WindowHandler()
	{
		glfwTerminate(); 
		delete _graphicsHandler;
	}
}