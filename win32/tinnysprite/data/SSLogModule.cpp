#include "SSCommon.h"

#ifdef USE_LOG_MODULE

#include <log4cpp/Category.hh>
#include "log4cpp/OstreamAppender.hh"
#include <log4cpp/FileAppender.hh>
#include <log4cpp/BasicLayout.hh>

#ifdef _DEBUG
#pragma comment(lib, "log4cppD")
#else
#pragma comment(lib, "log4cpp")
#endif

#include "SSLogModule.h"

using namespace SSDataDomain;

int SSLogModule::j = 0;
SSLogModule::SSLogModule()
{
	if ((++j) > 1)
		assert(false);
	// 1 instantiate an appender object that 
	// will append to a log file
	//appender = new log4cpp::FileAppender(
	//	"default",
	//	"/logs/testlog4cpp.log");
	appender = new log4cpp::OstreamAppender("default", &std::cout);

	// 2. Instantiate a layout object
	// Two layouts come already available in log4cpp
	// unless you create your own.
	// BasicLayout includes a time stamp
    log4cpp::Layout* layout = new log4cpp::BasicLayout();

	// 3. attach the layout object to the 
	// appender object
	appender->setLayout(layout);

	// 4. Instantiate the category object
	// you may extract the root category, but it is
	// usually more practical to directly instance
	// a child category
	category = &log4cpp::Category::getInstance("SSDataDomain");

	// 5. Step 1 
	// an Appender when added to a category becomes
	// an additional output destination unless 
	// Additivity is set to false when it is false,
	// the appender added to the category replaces
	// all previously existing appenders
	category->setAdditivity(false);

	// 5. Step 2
	// this appender becomes the only one
	category->setAppender(appender);

	// 6. Set up the priority for the category
	// and is given INFO priority
	// attempts to log DEBUG messages will fail
	category->setPriority(log4cpp::Priority::DEBUG);
}

SSLogModule::~SSLogModule()
{
	// clean up and flush all appenders
	log4cpp::Category::shutdown();
}

SSLogModule* SSLogModule::log_module = NULL;
SSLock SSLogModule::creation_lock;
SSLogModule* SSLogModule::instance()
{
	SYNCHRONIZED_SCOPE_WITH(&creation_lock);

	if (log_module == NULL)
		log_module = new SSLogModule();

	return log_module;
}

void SSLogModule::initialize()
{
	SSLogModule::instance();
}

void SSLogModule::dispose()
{
	SYNCHRONIZED_SCOPE_WITH(&creation_lock);
	
	if (log_module == NULL)
		return;

	delete log_module;
	log_module = NULL;
}
CategoryStream SSLogModule::debug()
{
	return SSLogModule::instance()->GetCategory().debugStream();
}
CategoryStream SSLogModule::info()
{
	return SSLogModule::instance()->GetCategory().infoStream();
}
CategoryStream SSLogModule::warn()
{
	return SSLogModule::instance()->GetCategory().warnStream();
}
CategoryStream SSLogModule::error()
{
	return SSLogModule::instance()->GetCategory().errorStream();
}

log4cpp::Category& SSLogModule::GetCategory()
{
	return *this->category;
}
SSMethodTraceLog::SSMethodTraceLog(const char* m)
{
	this->msg.assign(m);
	SSLogModule::instance()->GetCategory() << log4cpp::Priority::DEBUG << ">> " << msg << log4cpp::eol;
}
SSMethodTraceLog::SSMethodTraceLog(string& msg)
{
	this->msg = msg;
	SSLogModule::instance()->GetCategory() << log4cpp::Priority::DEBUG << ">> " << msg << log4cpp::eol;
}
SSMethodTraceLog::~SSMethodTraceLog()
{
	SSLogModule::instance()->GetCategory() << log4cpp::Priority::DEBUG << "<< " << msg << log4cpp::eol;
}

#endif