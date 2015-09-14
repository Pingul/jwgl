#ifndef _SIMULATION_INSTANT_
#define _SIMULATION_INSTANT_ 

#include <glm/glm.hpp>
#include <vector>

// For now it only consists of a time and a bunch of atoms
class SimulationInstant
{
	public:
		SimulationInstant() = delete;
		~SimulationInstant();

		SimulationInstant(double time = 0.0) :
			_time{time} {}
		SimulationInstant(std::vector<glm::vec3>* positions, double time = 0.0) :
			_time{time}, _positions{positions} {}

		void setPositions(std::vector<glm::vec3>* positions) { _positions = positions; }
		glm::vec3 positionFor(int index);
		void print();
		double timestamp() { return _time; }
		int nbrObjects();

	private:
		double _time{0.0};
		std::vector<glm::vec3>* _positions;

};

#endif