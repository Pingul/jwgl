#ifndef	_SIMULATION_
#define _SIMULATION_

#include <vector>
#include <glm/glm.hpp>
#include "simulationInstant.hpp"

class WorldObject;

class Simulation 
{
	public:
		~Simulation();
		Simulation(const char* file); // Read the simulation data from file

		void start(float t);
		void restart(float t);
		void increaseVisualizationSpeed();
		void decreaseVisualizationSpeed();
		void updatePositions(std::vector<WorldObject*>& objects, float t);
		glm::mat4 simulationTranslation();

		int nbrObjects(); // Used temporarily when we now that no additions of atoms will be made during simulation

	private:
		double _timeDelta{-1.0};
		double _visualizationSpeed{-1.0};
		glm::vec3 _dimensions{0, 0, 0};
		std::vector<SimulationInstant*>* _instants;
		int _currentInstantIndex{-1};
		double _localtime{-1.0};
		double _timeoffset{-1.0};

		int nextInstantIndex(double simulationTime);
		void validateSimulation();
};

#endif