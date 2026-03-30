#include <nexilis/logger/log.hh>
#include <nexilis/server/command/command_result.hh>

namespace nexilis::server
{

std::string asString(CommandResult res)
{
    switch (res)
    {
        case CommandResult::error:
            return "error";
        case CommandResult::failure:
            return "failure";
        case CommandResult::invalid_input:
            return "invalid_input";
        case CommandResult::success:
            return "success";
        case CommandResult::not_found:
            return "not_found";
        case CommandResult::unauthorized:
            return "unauthorized";
        case CommandResult::unimplemented:
            return "unimplemented";
    }
    return "";
}

bool checkResult(CommandResult result)
{
    switch (result)
    {
        case CommandResult::success:
            return true;

        case CommandResult::unauthorized:
            Log::error("Unauthorized");
            break;

        case CommandResult::unimplemented:
            Log::error("Unimplemented");
            break;

        case CommandResult::not_found:
            Log::error("Not found");
            break;

        case CommandResult::invalid_input:
            Log::error("Invalid input");
            break;

        case CommandResult::error:
            Log::error("Error");
            break;

        case CommandResult::failure:
            Log::error("Failure");
            break;
    }
    return false;
}

} // namespace nexilis::server
