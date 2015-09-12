#include "simulation.hpp"
#include "SIMFileReader.hpp"

#include <map>
#include <string>
#include <iostream>
#include <exception>

Simulation::Simulation(const char* file)
{
	SIMFileReader reader{};
	auto settings = new std::map<std::string, double>();
	reader.readSettings(file, *settings);
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
}