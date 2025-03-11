#ifndef NEXILIS_LOGGER_FUNCTION_HANDLER_HH
#define NEXILIS_LOGGER_FUNCTION_HANDLER_HH

#include <nexilis/logger/base_handler.hh>

#include <functional>

namespace nexilis::logger
{

class FunctionHandler : public BaseHandler
{
public:
    /// Constructor.
    explicit FunctionHandler(const std::function<void(LogLevel, const std::string&)>& function)
        : m_function(function)
    {
    }

    void emit(LogLevel logLevel, const std::string& data) override
    {
        m_function(logLevel, data);
    }

    bool operator==(const BaseHandler& other) const override
    {
        return this == &other;
    }

private:
    std::function<void(LogLevel, const std::string&)> m_function;
};

} // namespace nexilis::logger

#endif
