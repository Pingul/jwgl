#ifndef	_SIMULATION_
#define _SIMULATION_

class Simulation 
{
	public:
		Simulation(const char* file); // Read the simulation data from file

	private:
		double timeDelta{-1.0};
		double timeStart{0.0};
		double visualizationSpeed{-1.0};

		void validateSimulation();
};

#endif