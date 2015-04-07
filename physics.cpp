#include "misc.hpp"
#include "physics.hpp"
#include "models.hpp"
#include "terrain.hpp"

#include <glm/glm.hpp>

Physics::Physics()
{
	_gravity = glm::vec3(0, -1, 0);
}

void Physics::registerObjects(std::vector<WorldObject*>* objects)
{
	_worldObjects = objects;	
}

void Physics::calculatePositions(float t)
{
	float deltaT = t - _lastTime;
	findCollisions();
	// for (WorldObject* &obj : *_worldObjects)
	// {
	// 	if (obj->isAffectedByGravity())
	// 		obj->accelerate(obj->velocity() + deltaT * _gravity);

	// 	obj->move(obj->at() + deltaT*obj->velocity());
	// }
	_lastTime = t;
}

void print(glm::vec3 vec)
{
	std::cout << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}" << std::endl;
}

void Physics::findCollisions()
{
	WorldObject* sphere = _worldObjects->at(0);
	Terrain* terrain = dynamic_cast<Terrain*>(_worldObjects->at(1));

	int sphereRadius = 1;

	for (int x = 0; x < terrain->width() - 1; ++x)
	{
		for (int z = 0; z < terrain->depth() - 1; ++z)
		{
			int index[] = 
			{
				x + z*terrain->width(),
				x + 1 + z*terrain->width(),
				x + (z + 1)*terrain->width(),
			};

			glm::vec3 vertexLocation(terrain->_model->vertexArray()[3*index[0]], terrain->_model->vertexArray()[3*index[0] + 1], terrain->_model->vertexArray()[3*index[0] + 2]);
			glm::vec3 vertexNormal(terrain->_model->normalArray()[3*index[0]], terrain->_model->normalArray()[3*index[0] + 1], terrain->_model->normalArray()[3*index[0] + 2]);

			float val = glm::dot(vertexLocation, vertexNormal) - glm::dot(sphere->at(), vertexNormal);
			if (sphereRadius > val && val > 0)
			{
				std::cout << val << "/";
			}
		}
	}
	std::cout << std::endl;
}