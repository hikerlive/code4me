/*--------------------------------------------------------------------------------*\

     文件：  logger.h
     作者：  姚松文
     日期：  2010年10月20日
     说明：  无

\*--------------------------------------------------------------------------------*/

#ifndef LOGGER_H
#define LOGGER_H

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/helpers/properties.h>
#include <log4cxx/helpers/fileinputstream.h>

void initialize_logger();

#define DEFINE_LOGGER(x) \
    static log4cxx::LoggerPtr getLogger() { \
        static log4cxx::LoggerPtr _logger = NULL; \
        if (_logger == NULL) { \
            initialize_logger(); \
            _logger = log4cxx::Logger::getLogger((x)); \
        } \
        return _logger; \
    }

#define logger getLogger()

#define SSLOG_DEBUG(msg) LOG4CXX_DEBUG(getLogger(), msg)
#define SSLOG_INFO(msg) LOG4CXX_INFO(getLogger(), msg)
#define SSLOG_TRACE(msg) LOG4CXX_TRACE(getLogger(), msg)
#define SSLOG_ERROR(msg) LOG4CXX_ERROR(getLogger(), msg)

#define __SD_CLIENT_DLL_INTERNAL_USE__
#include <ISessionDomainClient.h>

#ifdef _UNICODE
#define TMessageBuffer WideMessageBuffer
#else
#define TMessageBuffer CharMessageBuffer
#endif //_UNICODE

#define LogStream std::basic_ostream<TCHAR>

LogStream& log(LogStream& oss, CallBackCMDID id);
LogStream& log(LogStream& oss, SSResultCode result);
LogStream& log(LogStream& oss, ShareType type);
LogStream& log(LogStream& oss, SSMessageType type);
LogStream& log(LogStream& oss, SSGroupT* p, int count);
LogStream& log(LogStream& oss, SSContactsT* p, int count);
LogStream& log(LogStream& oss, SSIconT* p, int count);
LogStream& log(LogStream& oss, SSProfileT* p, int count);
LogStream& log(LogStream& oss, SSLastTimeT* p, int count);
LogStream& log(LogStream& oss, SSUserExistT* p, int count);
LogStream& log(LogStream& oss, SSGroupMembersT* p, int count);
LogStream& log(LogStream& oss, ShareMsg* p, int count);
#endif // LOGGER_H