#include "SIMFileReader.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <exception>

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

	std::string value{statement.substr(statement.find('=') + 1, std::string::npos)};
	return std::stod(value);
}

void SIMFileReader::readSettings(const char* file, std::map<std::string, double>& settings)
{
	std::string line;
	std::ifstream simFile{file};
	if (simFile.is_open())
	{
		while (getline(simFile, line))
		{
			std::string trimmedLine{trim(strip(line))};
			if (isStatement(trimmedLine))
			{
				std::string variable{statementVariable(trimmedLine)};
				double value;
				if (variable.compare("t") == 0) // not a setting, exit
					break;

				try
				{
					value = statementValue(trimmedLine);
					settings.insert(std::pair<std::string, double>{variable, value});
				}
				catch(...)
				{
					std::cout << "Could not properly parse '" + line + '"' << std::endl;
				}
			}
		}
		simFile.close();
	}
}


