#include "misc.hpp"
#include "physics.hpp"
#include "models.hpp"

Physics::Physics()
{
	_gravity = glm::vec3(0, -1, 0);
}

void Physics::registerObjects(std::vector<WorldObject*>* objects)
{
	_worldObjects = objects;	
}

void Physics::generateNextPosition(float t)
{
	float deltaT = t - _lastTime;
	for (WorldObject* &obj : *_worldObjects)
	{
		if (obj->isAffectedByGravity())
			obj->accelerate(obj->velocity() + deltaT * _gravity);

		obj->move(obj->at() + deltaT * obj->velocity());
	}
	_lastTime = t;
}