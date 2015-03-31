#ifndef _WINDOW_HANDLER_
#define _WINDOW_HANDLER_

#include <GLFW/glfw3.h>

#include "GraphicsHandler.hpp"
#include <iostream>

namespace jwgl 
{
	class WindowHandler
	{
		public:
			WindowHandler(GraphicsHandler* graphicsHandler);
			~WindowHandler();

			void createWindow();
			void displayWindow();

		private:
			GLFWwindow* _window;
			GraphicsHandler* _graphicsHandler;
	};
}

#endif