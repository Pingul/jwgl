#include "misc.hpp"
#include "physics.hpp"
#include "terrain.hpp"

#include <glm/glm.hpp>

Physics::Physics()
{
	_gravity = glm::vec3(0, -9, 0);
}

void Physics::registerObjectManager(WorldObjectManager* objects)
{
	_objectManager = objects;	
}

void Physics::calculatePositions(float t)
{
	float deltaT = t - _lastTime;
	findCollisions(); // Updates direction of velocity for affected objects as well
	for (WorldObject* &obj : *_objectManager->objects())
	{
		if (obj->isAffectedByGravity())
			obj->accelerate(obj->velocity() + deltaT * _gravity);

		obj->move(obj->at() + deltaT*obj->velocity());
	}
	_lastTime = t;
}

void Physics::findCollisions()
{
	if (_objectManager->terrain()->empty())
		return;
	
	Terrain* terrain = _objectManager->terrain()->at(0);

	for (auto &sphere : *_objectManager->objects())
	{
		int minX = floor(sphere->at().x - sphere->radius());
		int maxX = ceil(sphere->at().x + sphere->radius());
		minX = minX > 0 ? minX : 0;
		maxX = maxX > terrain->width() - 1 ? minX : maxX;

		int minZ = floor(sphere->at().z - sphere->radius());
		int maxZ = ceil(sphere->at().z + sphere->radius());
		minZ = minZ > 0 ? minZ : 0;
		maxZ = maxZ > terrain->width() - 1 ? minZ : maxZ;

		for (int x = minX; x < maxX; ++x)
		{
			for (int z = minZ; z < maxZ; ++z)
			{
				// defines the first vertex
				glm::vec3 origin = terrain->vertexAt(x, z);
				glm::vec3 edge1 = terrain->vertexAt(x, z + 1) - origin;
				glm::vec3 edge2 = terrain->vertexAt(x + 1, z + 1) - origin;
				glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

				float val = glm::dot(normal, sphere->at() - origin);
				if (sphere->radius() > val && val > -sphere->radius())
				{
					// The object penetrates the plane defined by the vertex. Is it in fact inside of the vertex?
					glm::vec3 cutPoint = sphere->at() - val*normal;
					glm::vec3 s = cutPoint - origin;

					float param2 = (s.x*edge1.z - s.z*edge1.x)/(edge2.x*edge1.z - edge2.z*edge1.x);
					float param1 = (s.z - param2*edge2.z)/edge1.z;
					if (param1 + param2 <= 1 && param1 >= 0 && param2 >= 0)
					{
						// Move out and change velocity accordingly
						sphere->move(cutPoint + sphere->radius()*normal);
						sphere->accelerate(sphere->elasticity()*terrain->elasticity()*glm::reflect(sphere->velocity(), normal));
						break; // Can't return here as we then don't calculate collission for the next spheres
					}
				}

				// Same thing, next vertex
				edge1 = edge2; // we want right-hand coordinate system
				edge2 = terrain->vertexAt(x + 1, z) - origin;
				normal = glm::normalize(glm::cross(edge1, edge2));

				val = val = glm::dot(normal, sphere->at() - origin);
				if (sphere->radius() > val && val > -sphere->radius())
				{
					glm::vec3 cutPoint = sphere->at() - val*normal;
					glm::vec3 s = cutPoint - origin;

					float param2 = (s.x*edge1.z - s.z*edge1.x)/(edge2.x*edge1.z - edge2.z*edge1.x);
					float param1 = (s.z - param2*edge2.z)/edge1.z;
					if (param1 + param2 <= 1 && param1 >= 0 && param2 >= 0)
					{
						sphere->move(cutPoint + sphere->radius()*normal);
						sphere->accelerate(sphere->elasticity()*terrain->elasticity()*glm::reflect(sphere->velocity(), normal));
					}
				}
			}
		}
	}
}