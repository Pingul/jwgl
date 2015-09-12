#ifndef	_SIMULATION_
#define _SIMULATION_

class Simulation 
{
	public:
		Simulation(const char* file); // Read the simulation data from file

	private:
		double timeDelta;
		double timeStart;
		double visualizationSpeed;
};

#endif