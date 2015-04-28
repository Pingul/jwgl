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
	findCollisions();
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
	Terrain* terrain = _objectManager->terrain()->at(0);

	for (int x = 0; x < terrain->width() - 1; ++x)
	{
		for (int z = 0; z < terrain->depth() - 1; ++z)
		{
	// for (int x = 130; x < 132; ++x)
	// {
	// 	for (int z = 130; z < 132; ++z)
	// 	{
			// defines the first vertex
			glm::vec3 origin = terrain->vertexAt(x, z);
			glm::vec3 edge1 = terrain->vertexAt(x, z + 1) - origin;
			glm::vec3 edge2 = terrain->vertexAt(x + 1, z + 1) - origin;
			glm::vec3 normal = glm::cross(edge1, edge2);

			for (auto &sphere : *_objectManager->objects())
			{
				float val = glm::dot(normal, sphere->at() - origin);
				if (sphere->radius() > val && val > -sphere->radius())
				{
					// The object penetrates the plane defined by the vertex. Is it in fact inside of the vertex?
					glm::vec3 cutPoint = sphere->at() - val*normal;
					glm::vec3 s = cutPoint - origin;

					float param2 = (s.x*edge1.z - s.z*edge1.x)/(edge2.x*edge1.z - edge2.z*edge1.x);
					float param1 = (s.z - param2*edge2.z)/edge1.z;
					// std::cout << x << "." << z << " - " << "p1: " << param1 << " || " << param2 << std::endl;
					if (param1 + param2 <= 1 && param1 >= 0 && param2 >= 0)
					{
						// Move out and change velocity accordingly
						sphere->move(cutPoint + sphere->radius()*normal);
						sphere->accelerate(sphere->elasticity()*terrain->elasticity()*glm::reflect(sphere->velocity(), normal));
						// std::cout << "v: {" << sphere->velocity().x << ", " << sphere->velocity().y << "," << sphere->velocity().z << "}" << std::endl;
						return;
					}
				}

				// Same thing, next vertex
				edge1 = edge2; // we want right system
				edge2 = terrain->vertexAt(x + 1, z) - origin;
				normal = glm::cross(edge1, edge2);

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
						return;
					}
				}
			}
		}
	}
}