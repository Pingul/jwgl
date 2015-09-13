#include "simulationInstant.hpp"
#include <iostream>
#include <glm/glm.hpp>

void SimulationInstant::print()
{
	std::cout << "Instant: " << time_ << std::endl;
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

SimulationInstant::~SimulationInstant()
{
	delete positions_;
}