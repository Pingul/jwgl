#include "SIMFileReader.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <exception>
#include <sstream>
#include <glm/glm.hpp>
#include "SimulationInstant.hpp"

std::string trim(const std::string& str, const std::string& whitespace = " \t")
{
	size_t strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return "";

	size_t strEnd = str.find_last_not_of(whitespace);
	if (strEnd == std::string::npos)
		return "";

	return str.substr(strBegin, strEnd - strBegin + 1);
}

std::vector<std::string> split(const std::string& str, std::vector<std::string>& items, const char delimiter = ' ')
{
	std::stringstream stream{str};
	std::string item;
	while (std::getline(stream, item, delimiter))
	{
		items.push_back(item);
	}
	return items;
}

std::string strip(const std::string& str, const std::string& comment = "#")
{
	size_t pos = str.find_first_of(comment);
	return str.substr(0, pos);
}

// A statement is on the form 'variable = value'
bool isStatement(const std::string& str)
{
	std::regex pattern{"[A-Za-z0-9_]+?([ \t]*)="};
	return std::regex_search(str, pattern);
}

std::string statementVariable(const std::string& statement)
{
	if (!isStatement(statement))
		throw std::runtime_error{"Expected statement"};

	std::string variable{statement.substr(0, statement.find('='))};
	return trim(variable);
}

double statementValue(const std::string& statement)
{
	if (!isStatement(statement))
		throw std::runtime_error{"Expected statement"};

	std::string strValue{statement.substr(statement.find('=') + 1, std::string::npos)};
	double value;
	try
	{
		value = std::stod(strValue);
	}
	catch(...)
	{
		throw std::runtime_error{"Could not properly parse '" + statement + "' : expected double"};
	}
	
	return value;
}

glm::vec3 readVector(const std::string& str)
{
	std::vector<std::string> items;
	split(str, items);
	if (items.size() != 3)
		throw std::runtime_error{"Expected a vector, received '" + str + '"'};

	glm::vec3 position;
	try
	{
		position.x = std::stod(items.at(0));
		position.y = std::stod(items.at(1));
		position.z = std::stod(items.at(2));
	}
	catch(...)
	{
		throw std::runtime_error{"Could not parse the vector: '" + str + '"'};
	}
	return position;
}

void SIMFileReader::readFile(const char* file, std::map<std::string, double>& settings, std::vector<SimulationInstant*>& instants)
{
	std::string line;
	std::ifstream simFile{file};
	SimulationInstant* instant{nullptr};
	std::vector<glm::vec3>* positions{nullptr};
	if (simFile.is_open())
	{
		while (getline(simFile, line))
		{
			std::string trimmedLine{trim(strip(line))};
			if (trimmedLine.empty())
				continue;

			if (isStatement(trimmedLine))
			{
				std::string variable{statementVariable(trimmedLine)};
				double value{statementValue(trimmedLine)};

				if (variable.compare("t") == 0) // not a setting
				{
					if (instant != nullptr)
					{
						instant->setPositions(positions);
						instants.push_back(instant);
						if (instants.size() % 10 == 0)
							std::cout << instants.size() << " instants read" << std::endl;
					}

					instant = new SimulationInstant{value}; // will be the time of the instant
					positions = new std::vector<glm::vec3>{};
				} 
				else
				{
					settings.insert(std::pair<std::string, double>{variable, value});
				}
			}
			else if (positions != nullptr) // the line should be a vector declaration, checking for nullptr so we make sure it is initialized. Might be a little unsafe, but hey
			{
				positions->push_back(readVector(trimmedLine));
			}
			else
			{
				throw std::runtime_error{"Could not parse '" + line + "'"};
			}
		}
		if (instant != nullptr)
		{
			instant->setPositions(positions);
			instants.push_back(instant);
			instant = nullptr;
		}
		simFile.close();
	}
}

