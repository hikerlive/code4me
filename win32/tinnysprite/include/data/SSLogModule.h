#ifndef __SSLOGMODULE_H__
#define __SSLOGMODULE_H__

#include <string>
using namespace std;

#include <log4cpp/Category.hh>
using namespace log4cpp;

#include "SSConcurrency.h"

namespace SSDataDomain
{

///////////////////////////////////////////////////////////////////////////////
class SSLogModule
{
private:
	Appender* appender;
	Category* category;

	static int j;

public:
	SSLogModule();
	~SSLogModule();
	Category& GetCategory();

public:
	static SSLogModule* instance();
	static void initialize();
	static void dispose();

public:
	static CategoryStream debug();
	static CategoryStream info();
	static CategoryStream warn();
	static CategoryStream error();

private:
	static SSLogModule* log_module;
	static SSLock creation_lock;
};

class SSMethodTraceLog
{
private:
	string msg;
public:
	SSMethodTraceLog(const char* msg);
	SSMethodTraceLog(string& msg);
	~SSMethodTraceLog();
};

} // namespace

#endif