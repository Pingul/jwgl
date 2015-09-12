#include "SIMFileReader.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

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

// std::string statementVariable(const std::string& str)
// {
// 	if (!isStatement(str))
// 		throw new 
// }

void SIMFileReader::readSettings(const char* file, std::map<std::string, double>& settings)
{
	std::string line;
	std::ifstream simFile{file};
	if (simFile.is_open())
	{
		while (getline(simFile, line))
		{
			line = trim(strip(line));
			if (isStatement(line))

			if (!line.empty())
				std::cout << line << (isStatement(line) ? "matched" : "") << std::endl;
		}
		simFile.close();
	}
}


