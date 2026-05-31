#ifndef NEXILIS_CLIENT_COMMAND_ERROR_HH
#define NEXILIS_CLIENT_COMMAND_ERROR_HH

#include <nexilis/client/base_api_command.hh>

#include <tuple>
#include <type_traits>

namespace nexilis::client
{

template <typename... Args>
class ErrorCommand : public BaseAPICommand
{
public:
    explicit ErrorCommand(ReadResult result, Args&&... args)
        : m_result(result),
          m_args(std::forward<Args>(args)...)
    {
    }

    static std::unique_ptr<ErrorCommand> make_unique(ReadResult result, Args&&... args)
    {
        return std::make_unique<ErrorCommand>(result, std::forward<Args>(args)...);
    }

    ReadResult execute(ClientAPI&, ClientAPI::ClientAPIData&) override
    {
        return m_result;
    }

private:
    ReadResult m_result;
    std::tuple<std::decay_t<Args>...> m_args;
};

} // namespace nexilis::client

#endif
