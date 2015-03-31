#ifndef _GRAPHICS_HANDLER_
#define _GRAPHICS_HANDLER_

#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif


#include <GLFW/glfw3.h>

namespace jwgl
{
	// Abstract super class. Subclass to implement application 
	class GraphicsHandler 
	{
		public:
			virtual ~GraphicsHandler() = default;
		
			virtual void drawFrame(float t) = 0;
			virtual void init() = 0;
			virtual void registerWindow(GLFWwindow* window) { this->_window = window; }

		protected:
			GLFWwindow* _window; // Necessary for key input
	};
}
#endif