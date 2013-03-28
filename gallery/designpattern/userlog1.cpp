#include "logfile.h"
#include "userlog1.h"

void f()
{
	logfile() << __FILE__<< std::endl;
}