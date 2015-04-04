#ifndef _PHYSICS_
#define _PHYSICS_

#include <vector>
#include <glm/glm.hpp>

class WorldObject;

class Physics
{
	public:
		Physics();
		~Physics() = default;

		void registerObjects(std::vector<WorldObject*>* objects);
		void generateNextPosition(float t);
	
	private:
		float _lastTime;
		glm::vec3 _gravity;
		std::vector<WorldObject*>* _worldObjects;	

};

#endif