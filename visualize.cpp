
#include "misc.hpp"
#include "WindowHandler.hpp"
#include "VisualizationGraphicsHandler.hpp"

int main()
{
	try
	{
		jwgl::WindowHandler handler(new VisualizationGraphicsHandler);
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
