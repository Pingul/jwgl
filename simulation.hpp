#ifndef	_SIMULATION_
#define _SIMULATION_

#include <vector>
#include "simulationInstant.hpp"

class WorldObject;

class Simulation 
{
	public:
		~Simulation();
		Simulation(const char* file); // Read the simulation data from file

		// void start();
		void updatePositions(std::vector<WorldObject*>& objects, float t);

		int nbrObjects(); // Used temporarily when we now that no additions of atoms will be made during simulation

	private:
		double timeDelta{-1.0};
		double timeStart{0.0};
		double visualizationSpeed{-1.0};
		std::vector<SimulationInstant*>* instants;
		int currentInstantIndex{-1};

		void validateSimulation();
};

#endif