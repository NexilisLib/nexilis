#ifndef NEXILIS_LOGGER_BASEHANDLER_HH
#define NEXILIS_LOGGER_BASEHANDLER_HH

#include <nexilis/logger/log_level.hh>

#include <string>

namespace nexilis
{

class BaseHandler
{
public:
	/// Destructor.
	virtual ~BaseHandler(){}
	
	// Handle logs.
	// \param logLevel The log level of the message.
	// \param data The data of the given message.
	virtual void emit(const LogLevel& logLevel, const std::string& data) = 0;
};

}

#endif
