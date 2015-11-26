#include "simulation.hpp"
#include "SIMFileReader.hpp"

#include <map>
#include <string>
#include <iostream>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>
#include "simulationInstant.hpp"
#include "models.hpp"

#include <thread>
#include <chrono>

Simulation::Simulation(const char* file)
{
	auto settings = new std::map<std::string, double>{};
	_instants = new std::vector<SimulationInstant*>{};
	std::thread thread{&fileIO::SIM::read, file, std::ref(*settings), std::ref(*_instants)};
	thread.detach();
	std::this_thread::sleep_for(std::chrono::milliseconds(500)); // should be enough
	
	for (const std::pair<std::string, double>& setting : *settings)
	{
		if (setting.first.compare("timeDelta") == 0)
			_timeDelta = setting.second;
		else if (setting.first.compare("visualizationSpeed") == 0)
			_visualizationSpeed = setting.second;
		else if (setting.first.compare("dimensions.x") == 0)
			_dimensions.x = setting.second;
		else if (setting.first.compare("dimensions.y") == 0)
			_dimensions.y = setting.second;
		else if (setting.first.compare("dimensions.z") == 0)
			_dimensions.z = setting.second;
		else
			std::cout << "Given setting was not found: '" << setting.first << '"' << std::endl;
	}
	std::cout << "here" << std::endl;
	validateSimulation();
	// delete settings;
}

void Simulation::validateSimulation()
{
	if (_timeDelta < 0.0)
		throw std::runtime_error{"timeDelta is not properly initialized"};

	if (_visualizationSpeed < 0.0)
		_visualizationSpeed = _timeDelta;
}

glm::mat4 Simulation::simulationTranslation()
{
	return glm::translate(glm::mat4(), -glm::vec3{_dimensions.x/2, _dimensions.y/2, _dimensions.z/2});
}

void Simulation::start(float t)
{
	if (_instants == nullptr || _instants->size() == 0)
		throw std::runtime_error{"The simulation has no simulation instants"};

	_localtime = 0.0;
	_timeoffset = t;
	_currentInstantIndex = 0;
}

void Simulation::restart(float t)
{
	_currentInstantIndex = 0;
	_timeoffset = t;
	_localtime = 0.0;
}

double Simulation::increaseVisualizationSpeed()
{
	_visualizationSpeed += 1;
	return _visualizationSpeed;
}

double Simulation::decreaseVisualizationSpeed()
{
	if (_visualizationSpeed - 1.0 > 0.0)
		_visualizationSpeed -= 1;
	return _visualizationSpeed;
}


int Simulation::nextInstantIndex(double t)
{
	SimulationInstant* currentInstant = _instants->at(_currentInstantIndex);
	double timeDiff = t - currentInstant->timestamp();
	double frameDiff = timeDiff/_timeDelta;
	int indexDiff = std::floor(frameDiff);
	return _currentInstantIndex + indexDiff;
} 

void Simulation::updatePositions(std::vector<WorldObject*>& objects, float t)
{
	_localtime = t - _timeoffset;
	int nextIndex = nextInstantIndex(_localtime*_visualizationSpeed);
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