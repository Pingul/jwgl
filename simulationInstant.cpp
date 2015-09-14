#include "simulationInstant.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <exception>

void SimulationInstant::print()
{
	std::cout << "Instant: " << _time << " size: " << _positions->size() << std::endl;
	int index = 1;
	for (const auto& position : *_positions)
	{
		std::cout << index << ": " << position.x << " " << position.y << " " << position.z << std::endl;
		index++;
	}
}

int SimulationInstant::nbrObjects()
{
	if (_positions == nullptr)
		return 0;
	return _positions->size();
}

glm::vec3 SimulationInstant::positionFor(int index)
{
	if (_positions == nullptr)
		throw std::runtime_error{"Positions have not been initialized"};
	return _positions->at(index);
}

SimulationInstant::~SimulationInstant()
{
	delete _positions;
}