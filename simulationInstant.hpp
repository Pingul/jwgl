#ifndef _SIMULATION_INSTANT_
#define _SIMULATION_INSTANT_ 

#include <glm/glm.hpp>
#include <vector>

// For now it only consists of a time and a bunch of atoms
class SimulationInstant
{
	public:
		SimulationInstant() = delete;
		~SimulationInstant() = default;

		SimulationInstant(double time = 0.0) :
			time_{time} {}
		SimulationInstant(std::vector<glm::vec3> positions, double time = 0.0) :
			time_{time}, positions_{positions} {}


	private:
		double time_{0.0};
		std::vector<glm::vec3> positions_;

};

#endif