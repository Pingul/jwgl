
#include "misc.hpp"
#include "WindowHandler.hpp"
#include "VisualizationGraphicsHandler.hpp"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Need file to use for visualisation" << std::endl;
		std::cout << "abort" << std::endl;
		return 0;
	}

	try
	{
		jwgl::WindowHandler handler(new VisualizationGraphicsHandler{argv[1]});
		handler.createWindow();
		handler.displayWindow();
	} 
	catch (const std::exception& error)
	{
		std::cout << "ERROR: " << error.what() << std::endl;
		std::cout << "abort" << std::endl;
	}

	return 0;
}
