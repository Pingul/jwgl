#ifndef _PHYSICS_
#define _PHYSICS_

#include <vector>
#include <glm/glm.hpp>
#include "models.hpp"

class Physics
{
	public:
		Physics();
		~Physics() = default;

		void registerObjectManager(WorldObjectManager* objects);
		void calculatePositions(float t);
	
	private:
		float _lastTime;
		glm::vec3 _gravity;
		WorldObjectManager* _objectManager;

		void findCollisions();
};

#endif