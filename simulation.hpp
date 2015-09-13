#ifndef	_SIMULATION_
#define _SIMULATION_

#include <vector>
#include "simulationInstant.hpp"

class Simulation 
{
	public:
		~Simulation();
		Simulation(const char* file); // Read the simulation data from file

	private:
		double timeDelta{-1.0};
		double timeStart{0.0};
		double visualizationSpeed{-1.0};
		std::vector<SimulationInstant*>* instants;

		void validateSimulation();
};

#endif