#include "simulationInstant.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <exception>

void SimulationInstant::print()
{
	std::cout << "Instant: " << time_ << " size: " << positions_->size() << std::endl;
	int index = 1;
	for (const auto& position : *positions_)
	{
		std::cout << index << ": " << position.x << " " << position.y << " " << position.z << std::endl;
		index++;
	}
}

int SimulationInstant::nbrObjects()
{
	if (positions_ == nullptr)
		return 0;
	return positions_->size();
}

glm::vec3 SimulationInstant::positionFor(int index)
{
	if (positions_ == nullptr)
		throw std::runtime_error{"Positions have not been initialized"};
	return positions_->at(index);
}

SimulationInstant::~SimulationInstant()
{
	delete positions_;
}