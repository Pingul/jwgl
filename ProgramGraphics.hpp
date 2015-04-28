#ifndef _PROGRAM_GRAPHICS_
#define _PROGRAM_GRAPHICS_

#include "GraphicsHandler.hpp"
#include <vector>

class ShaderManager;
class Camera;
class WorldObject;
class LightSourceLoader;
class Physics;
class TerrainGenerator;
class WorldObjectManager;

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
		TerrainGenerator* _terrainGenerator;
		WorldObjectManager* _objectManager;
		Physics* _physics;

		void setupOpenGL();
		void loadShaders();
		void loadModels();
		void loadLightSources();
		void setupCamera();
		void setupPhysics();
		void handleKeys();
		void handleMouseMovement();
		void handleSphereObjectMovement();
		void handleCameraMovement();
};

#endif