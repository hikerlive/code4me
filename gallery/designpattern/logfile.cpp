#include "logfile.h"

std::ofstream& logfile()
{
	static std::ofstream log("logfile.log");
	return log;
}