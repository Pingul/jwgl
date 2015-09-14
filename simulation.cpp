#include "simulation.hpp"
#include "SIMFileReader.hpp"

#include <map>
#include <string>
#include <iostream>
#include <exception>
#include "simulationInstant.hpp"
#include "models.hpp"

Simulation::Simulation(const char* file)
{
	SIMFileReader reader{};
	auto settings = new std::map<std::string, double>{};
	_instants = new std::vector<SimulationInstant*>{};
	reader.readFile(file, *settings, *_instants);
	for (const std::pair<std::string, double>& setting : *settings)
	{
		if (setting.first.compare("timeDelta") == 0)
			_timeDelta = setting.second;
		else if (setting.first.compare("timeStart") == 0)
			_timeStart = setting.second;
		else if (setting.first.compare("visualizationSpeed") == 0)
			_visualizationSpeed = setting.second;
		else
			std::cout << "Given setting was not found: '" << setting.first << '"' << std::endl;
	}
	validateSimulation();
	delete settings;
}

void Simulation::validateSimulation()
{
	if (_timeDelta < 0.0)
		throw std::runtime_error{"timeDelta is not properly initialized"};

	if (_visualizationSpeed < 0.0)
		_visualizationSpeed = _timeDelta;

	if (_instants != nullptr && _instants->size() > 0)
	{
		int nbrObjects{_instants->front()->nbrObjects()};
		for (auto& instant : *_instants)
		{
			if (instant->nbrObjects() != nbrObjects)
				throw std::runtime_error{"All simulation instants is not of the same length"};

			// instant->print();
		}
	}

}

void Simulation::start()
{
	if (_instants == nullptr || _instants->size() == 0)
		throw std::runtime_error{"The simulation has no simulation instants"};

	_currentInstantIndex = 0;
}

int Simulation::nextInstantIndex(double simulationTime)
{
	SimulationInstant* currentInstant = _instants->at(_currentInstantIndex);
	double timeDiff = simulationTime - currentInstant->timestamp();
	double frameDiff = timeDiff/_timeDelta;
	int indexDiff = std::floor(frameDiff);
	return _currentInstantIndex + indexDiff;
} 

void Simulation::updatePositions(std::vector<WorldObject*>& objects, float t)
{
	double simulationTime = t*_visualizationSpeed + _timeStart;
	int nextIndex = nextInstantIndex(simulationTime);
	if (nextIndex + 1 >= _instants->size())
	{
		// we can't update the positions
		static bool b = false;
		if (!b)
		{
			std::cout << "out of data" << std::endl;
			b = true;
		}
		return;
	}

	SimulationInstant* currentInstant = _instants->at(_currentInstantIndex);
	SimulationInstant* nextInstant = _instants->at(nextIndex);
	if (nextInstant != currentInstant)
	{
		for (int i = 0; i < objects.size(); i++) // objects and all instants are guaranteed to have the same .size()
		{
			objects.at(i)->move(nextInstant->positionFor(i));
		}
	}
}

int Simulation::nbrObjects()
{
	if (_instants == nullptr || _instants->size() == 0)
		throw std::runtime_error{"The simulation has no simulation instants"};

	return _instants->at(0)->nbrObjects();
}

Simulation::~Simulation()
{
	for (auto& instant : *_instants)
	{
		delete instant;
	}
	delete _instants;
}