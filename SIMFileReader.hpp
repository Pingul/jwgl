#ifndef _SIM_FILE_READER_
#define _SIM_FILE_READER_

#include <string>
#include <map>

class SIMFileReader
{
	public:
		SIMFileReader() = default;
		~SIMFileReader() = default;

		void readSettings(const char* file, std::map<std::string, double>& settings);
};

#endif