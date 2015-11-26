#ifndef _VISUALIZATION_GRAPHICS_HANDLER_
#define _VISUALIZATION_GRAPHICS_HANDLER_

#include "GraphicsHandler.hpp"
#include <string>

class LightSourceLoader;
class ShaderManager;
class DragCamera;
class WorldObjectManager;
class Simulation;

class VisualizationGraphicsHandler : public jwgl::GraphicsHandler
{
	public:
		VisualizationGraphicsHandler() = delete;
		VisualizationGraphicsHandler(const std::string& file);
		virtual ~VisualizationGraphicsHandler();

		virtual void init();
		virtual void drawFrame(float t);

	private:
		std::string _file;
		ShaderManager* _shaders;
		DragCamera* _camera;
		LightSourceLoader* _lightSources;
		WorldObjectManager* _worldObjects;
		Simulation* _simulation;

		void manageUserKeyInput(float t);

		void setupOpenGL();
		void loadShaders();
		void loadLightSources();
		void setupCamera();
		void handleMouseMovement();
};

#endif