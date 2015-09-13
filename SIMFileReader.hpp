#ifndef _SIM_FILE_READER_
#define _SIM_FILE_READER_

#include <string>
#include <map>
#include <vector>

class SimulationInstant;

class SIMFileReader
{
	public:
		SIMFileReader() = default;
		~SIMFileReader() = default;

		void readFile(const char* file, std::map<std::string, double>& settings, std::vector<SimulationInstant*>& instants);
};

#endif