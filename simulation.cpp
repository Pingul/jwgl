#include "simulation.hpp"
#include "SIMFileReader.hpp"

#include <map>
#include <string>

Simulation::Simulation(const char* file)
{
	SIMFileReader reader{};
	auto settings = new std::map<std::string, double>();
	reader.readSettings(file, *settings);
}