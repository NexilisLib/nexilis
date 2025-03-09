#include <nexilis/logger/logger.hh>
#include <nexilisc/logger/logger_c.h>

#include <nexilis/logger/file_handler.hh>

struct nexilis_logger_LoggerC
{
    nexilis::logger::Logger* logger;
};

static nexilis::logger::LogLevel toLogLevel(int level)
{
    return static_cast<nexilis::logger::LogLevel>(level);
}

nexilis_logger_LoggerC* nexilis_logger_create()
{
    nexilis_logger_LoggerC* loggerC = new nexilis_logger_LoggerC;
    loggerC->logger = new nexilis::logger::Logger();
    return loggerC;
}

void nexilis_logger_destroy(nexilis_logger_LoggerC* logger)
{
    if (logger && logger->logger)
    {
        logger->logger->clearHandlers();
        delete logger->logger;
        delete logger;
    }
}

void nexilis_logger_add_file_handler(nexilis_logger_LoggerC* logger, nexilis_logger_FileHandler* handler)
{
    if (logger && handler)
    {
        auto cpp_logger = logger->logger;
        auto cpp_handler = handler->handler;
        cpp_logger->addHandler(std::unique_ptr<nexilis::logger::FileHandler>(cpp_handler));

        handler->handler = nullptr;
    }
}

void nexilis_logger_add_console_handler(nexilis_logger_LoggerC* logger, nexilis_logger_ConsoleHandler* handler)
{
    if (logger && logger->logger && handler && handler->handler)
    {
        auto cpp_logger = logger->logger;
        auto cpp_handler = handler->handler;
        cpp_logger->addHandler(std::unique_ptr<nexilis::logger::ConsoleHandler>(cpp_handler));

        handler->handler = nullptr;
    }
}

void nexilis_logger_add_function_handler(nexilis_logger_LoggerC* logger, nexilis_logger_FunctionHandler* handler)
{
    if (logger && logger->logger && handler && handler->handler)
    {
        auto cpp_logger = logger->logger;
        auto cpp_handler = handler->handler;
        cpp_logger->addHandler(std::unique_ptr<nexilis::logger::FunctionHandler>(cpp_handler));

        handler->handler = nullptr;
    }
}

void nexilis_logger_remove_handler(nexilis_logger_LoggerC* logger, uint64_t handlerId)
{
    if (logger)
    {
        logger->logger->removeHandler(handlerId);
    }
}

void nexilis_logger_clear_handlers(nexilis_logger_LoggerC* logger)
{
    if (logger)
    {
        logger->logger->clearHandlers();
    }
}

int nexilis_logger_no_handlers(nexilis_logger_LoggerC* logger)
{
    return logger ? logger->logger->noHandlers() : 1;
}

void nexilis_logger_debug(nexilis_logger_LoggerC* logger, const char* message)
{
    if (logger && logger->logger && message)
    {
        logger->logger->debug(message);
    }
}

void nexilis_logger_info(nexilis_logger_LoggerC* logger, const char* message)
{
    if (logger && logger->logger && message)
    {
        logger->logger->info(message);
    }
}

void nexilis_logger_warning(nexilis_logger_LoggerC* logger, const char* message)
{
    if (logger && logger->logger && message)
    {
        logger->logger->warning(message);
    }
}

void nexilis_logger_error(nexilis_logger_LoggerC* logger, const char* message)
{
    if (logger && logger->logger && message)
    {
        logger->logger->error(message);
    }
}

void nexilis_logger_critical(nexilis_logger_LoggerC* logger, const char* message)
{
    if (logger && logger->logger && message)
    {
        logger->logger->critical(message);
    }
}

bool nexilis_logger_unset_level(nexilis_logger_LoggerC* logger, int level)
{
    if (logger)
    {
        return logger->logger->unsetLevel(toLogLevel(level));
    }
    return false;
}

bool nexilis_logger_set_level(nexilis_logger_LoggerC* logger, int level)
{
    if (logger)
    {
        return logger->logger->setLevel(toLogLevel(level));
    }
    return false;
}

bool nexilis_logger_get_level(nexilis_logger_LoggerC* logger, int level)
{
    if (logger)
    {
        return logger->logger->getLevel(toLogLevel(level));
    }
    return false;
}

bool nexilis_logger_set_minimum_level(nexilis_logger_LoggerC* logger, int level)
{
    if (logger)
    {
        return logger->logger->setMinimumLevel(toLogLevel(level));
    }
    return false;
}

void nexilis_logger_set_log_level(nexilis_logger_LoggerC* logger, uint8_t level)
{
    if (logger)
    {
        logger->logger->setLogLevel(level);
    }
}
