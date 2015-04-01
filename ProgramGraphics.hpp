#ifndef _PROGRAM_GRAPHICS_
#define _PROGRAM_GRAPHICS_

#include "GraphicsHandler.hpp"
#include <vector>

class ShaderManager;
class Camera;
class WorldObject;
class LightSourceLoader;

class ProgramGraphics : public jwgl::GraphicsHandler
{
	public:
		ProgramGraphics() = default;
		virtual ~ProgramGraphics();

		virtual void init();
		virtual void drawFrame(float t);

	private:
		ShaderManager* _shaders;
		Camera* _camera;
		LightSourceLoader* _lightSources;
		std::vector<WorldObject*> _worldObjects;

		void setupOpenGL();
		void loadShaders();
		void loadModels();
		void loadLightSources();
		void setupCamera();
		void handleKeys();
		void handleMouseMovement();
};

#endif