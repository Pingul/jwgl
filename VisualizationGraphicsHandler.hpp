#ifndef _VISUALIZATION_GRAPHICS_HANDLER_
#define _VISUALIZATION_GRAPHICS_HANDLER_

#include "GraphicsHandler.hpp"

class LightSourceLoader;
class ShaderManager;
class Camera;
class WorldObjectManager;
class Simulation;

class VisualizationGraphicsHandler : public jwgl::GraphicsHandler
{
	public:
		VisualizationGraphicsHandler() = default;
		virtual ~VisualizationGraphicsHandler();

		virtual void init();
		virtual void drawFrame(float t);

	private:
		ShaderManager* _shaders;
		Camera* _camera;
		LightSourceLoader* _lightSources;
		WorldObjectManager* _worldObjects;
		Simulation* _simulation;

		void setupOpenGL();
		void loadShaders();
		void loadLightSources();
		void setupCamera();
};

#endif