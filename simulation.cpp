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
	instants = new std::vector<SimulationInstant*>{};
	reader.readFile(file, *settings, *instants);
	for (const std::pair<std::string, double>& setting : *settings)
	{
		if (setting.first.compare("timeDelta") == 0)
			timeDelta = setting.second;
		else if (setting.first.compare("timeStart") == 0)
			timeStart = setting.second;
		else if (setting.first.compare("visualizationSpeed") == 0)
			visualizationSpeed = setting.second;
		else
			std::cout << "Given setting was not found: '" << setting.first << '"' << std::endl;
	}
	validateSimulation();
	delete settings;
}

void Simulation::validateSimulation()
{
	if (timeDelta < 0.0)
		throw std::runtime_error{"timeDelta is not properly initialized"};

	if (visualizationSpeed < 0.0)
		visualizationSpeed = timeDelta;

	if (instants != nullptr && instants->size() > 0)
	{
		int nbrObjects{instants->front()->nbrObjects()};
		for (auto& instant : *instants)
		{
			if (instant->nbrObjects() != nbrObjects)
				throw std::runtime_error{"All simulation instants is not of the same length"};

			// instant->print();
		}
	}

}

// void Simulation::start()
// {
// 	if (instants == nullptr || instants->size() == 0)
// 		throw std::runtime_error{"The simulation has no simulation instants"};

// 	// currentInstantIndex = 0;
// }

void Simulation::updatePositions(std::vector<WorldObject*>& objects, float t)
{
	SimulationInstant* nextInstant = instants->at(currentInstantIndex + 1);
	if (nextInstant->timestamp() < t)
	{
		std::cout << "changed instant" << std::endl;
		currentInstantIndex++;
		for (int i = 0; i < objects.size(); i++) // objects and all instants are guaranteed to have the same .size()
		{
			objects.at(i)->move(nextInstant->positionFor(i));
		}
	}
}

int Simulation::nbrObjects()
{
	if (instants == nullptr || instants->size() == 0)
		throw std::runtime_error{"The simulation has no simulation instants"};

	return instants->at(0)->nbrObjects();
}

Simulation::~Simulation()
{
	for (auto& instant : *instants)
	{
		delete instant;
	}
	delete instants;
}