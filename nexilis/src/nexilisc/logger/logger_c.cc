#include <nexilisc/logger/logger_c.h>

#include <nexilis/logger/console_handler.hh>
#include <nexilis/logger/file_handler.hh>
#include <nexilis/logger/logger.hh>

using namespace nexilis::logger;

extern "C"
{

    struct nexilis_logger_LoggerC
    {
        Logger* logger;
    };

    struct nexilis_logger_FileHandler
    {
        nexilis::logger::FileHandler* handler;
    };

    struct nexilis_logger_ConsoleHandler
    {
        nexilis::logger::ConsoleHandler* handler;
    };

    static LogLevel toLogLevel(int level)
    {
        return static_cast<LogLevel>(level);
    }

    nexilis_logger_LoggerC* nexilis_logger_create()
    {
        nexilis_logger_LoggerC* loggerC = new nexilis_logger_LoggerC;
        loggerC->logger = new Logger();
        return loggerC;
    }

    void nexilis_logger_destroy(nexilis_logger_LoggerC* logger)
    {
        if (logger)
        {
            logger->logger->clearHandlers();
            delete logger->logger;
            delete logger;
        }
    }

    nexilis_logger_FileHandler* nexilis_logger_FileHandler_create(const char* filename)
    {
        return reinterpret_cast<nexilis_logger_FileHandler*>(new nexilis::logger::FileHandler(filename));
    }

    void nexilis_logger_FileHandler_destroy(nexilis_logger_FileHandler* handler)
    {
        delete reinterpret_cast<FileHandler*>(handler);
    }
    void nexilis_logger_FileHandler_emit(nexilis_logger_FileHandler* handler, nexilis_logger_loglevel log_level, const char* data)
    {
        auto file_handler = reinterpret_cast<FileHandler*>(handler);
        file_handler->emit(static_cast<LogLevel>(log_level), data);
    }

    void nexilis_logger_add_file_handler(nexilis_logger_LoggerC* logger, nexilis_logger_FileHandler* handler)
    {
        if (logger && handler)
        {
            // Convert the C-style logger to the C++ logger.
            auto cpp_logger = reinterpret_cast<Logger*>(logger);

            // Convert the C-style file handler to the C++ file handler
            auto cpp_handler = reinterpret_cast<FileHandler*>(handler);

            // Add the handler to the logger
            cpp_logger->addHandler(std::unique_ptr<FileHandler>(cpp_handler));
        }
    }

    nexilis_logger_ConsoleHandler* nexilis_logger_ConsoleHandler_create()
    {
        return reinterpret_cast<nexilis_logger_ConsoleHandler*>(new ConsoleHandler());
    }
    void nexilis_logger_ConsoleHandler_destroy(nexilis_logger_ConsoleHandler* handler)
    {
        delete reinterpret_cast<ConsoleHandler*>(handler);
    }

    void nexilis_logger_ConsoleHandler_emit(nexilis_logger_ConsoleHandler* handler, nexilis_logger_loglevel log_level, const char* data)
    {
        auto console_handler = reinterpret_cast<ConsoleHandler*>(handler);
        console_handler->emit(static_cast<LogLevel>(log_level), data);
    }

    void nexilis_logger_add_console_handler(nexilis_logger_LoggerC* logger, nexilis_logger_ConsoleHandler* handler)
    {
        if (logger && handler)
        {
            auto cpp_logger = reinterpret_cast<Logger*>(logger);
            auto cpp_handler = reinterpret_cast<ConsoleHandler*>(handler);
            cpp_logger->addHandler(std::unique_ptr<ConsoleHandler>(cpp_handler));
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
        if (logger && message)
        {
            logger->logger->debug(message);
        }
    }

    void nexilis_logger_info(nexilis_logger_LoggerC* logger, const char* message)
    {
        if (logger && message)
        {
            logger->logger->info(message);
        }
    }

    void nexilis_logger_warning(nexilis_logger_LoggerC* logger, const char* message)
    {
        if (logger && message)
        {
            logger->logger->warning(message);
        }
    }

    void nexilis_logger_error(nexilis_logger_LoggerC* logger, const char* message)
    {
        if (logger && message)
        {
            logger->logger->error(message);
        }
    }

    void nexilis_logger_critical(nexilis_logger_LoggerC* logger, const char* message)
    {
        if (logger && message)
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

} // extern "C"
