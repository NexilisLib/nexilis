#include <nexilis/client/command/get/client_id.hh>
#include <nexilis/client/command/get/info_rooms.hh>
#include <nexilis/client/command/get/room_id.hh>
#include <nexilis/client/command/room/communication.hh>
#include <nexilis/client/command/room/gameitem.hh>
#include <nexilis/client/command/room/management.hh>
#include <nexilis/client/command/room/object2d.hh>
#include <nexilis/client/command/room/object3d.hh>
#include <nexilis/client/command/room/player2d.hh>
#include <nexilis/client/command/room/player3d.hh>
#include <nexilis/client/command/set/port.hh>
#include <nexilis/client/command/set/username.hh>
#include <nexilis/client/command_parser.hh>

namespace nexilis::client
{

template <typename... Args>
std::unique_ptr<ErrorCommand<Args...>> makeError(ReadResult result, Args&&... args)
{
    return std::make_unique<ErrorCommand<Args...>>(result, std::forward<Args>(args)...);
}

std::unique_ptr<BaseAPICommand> CommandParser::parse(const boost::json::object& json)
{
    if (!json.contains("command") || !json.contains("type"))
    {
        return makeError(ReadResult::parsing_failed, "no \"command\" or \"type\" found!");
    }

    auto cmd_type = commandTypeFromString(json.at("command").as_string().c_str());
    auto type = json.at("type").as_string();

    switch (cmd_type)
    {
        case CommandType::setting:
            return parseSettingCommand(json, type);
        case CommandType::getting:
            return parseGettingCommand(json, type);
        case CommandType::room:
            return parseRoomCommand(json, type);
        case CommandType::error:
            return parseErrorCommand(type);
        case CommandType::authentication:
        default:
            return makeError(ReadResult::parsing_failed, "command parsing failed to execute");
    }
    return makeError(ReadResult::parsing_failed, "command type not found");
}

std::unique_ptr<BaseAPICommand> CommandParser::parseSettingCommand(const boost::json::object& json, std::string_view type)
{
    if (type == "username")
    {
        std::string username = json.at("username").as_string().c_str();

        uint64_t client_id = 0;
        if (json.contains("client_id"))
            client_id = Util::toUint64(json.at("client_id"));

        return std::make_unique<SetUsernameCommand>(client_id, username);
    }
    else if (type == "port")
    {
        std::string protocol = "boost_tcp";
        if (json.contains("protocol"))
            protocol = json.at("protocol").as_string().c_str();

        uint16_t port = 0;
        if (json.contains("port"))
            port = static_cast<uint16_t>(Util::toUint64(json.at("port")));
        else if (json.contains("boost_tcp_port"))
            port = static_cast<uint16_t>(Util::toUint64(json.at("boost_tcp_port")));

        return std::make_unique<SetPortCommand>(protocol, port);
    }
    return makeError(ReadResult::parsing_failed, "cannot find set command type");
}

std::unique_ptr<BaseAPICommand> CommandParser::parseGettingCommand(const boost::json::object& json, std::string_view type)
{
    if (type == "client_id")
    {
        uint64_t client_id = Util::toUint64(json.at("client_id"));
        return std::make_unique<GetClientIdCommand>(client_id);
    }

    else if (type == "room_id")
    {
        uint64_t room_id = Util::toUint64(json.at("room_id"));
        return std::make_unique<GetRoomIdCommand>(room_id);
    }

    else if (type == "info_rooms")
    {
        if (json.contains("rooms"))
        {
            return std::make_unique<GetInfoRoomsCommand>(json.at("rooms"));
        }

        return makeError(ReadResult::invalid_input);
    }

    return makeError(ReadResult::parsing_failed, "cannot find get command type");
}

std::unique_ptr<BaseAPICommand> CommandParser::parseRoomCommand(const boost::json::object& json, std::string_view type)
{
    // Parse room commands based on 'type' and 'action' fields
    if (!json.contains("action"))
    {
        return makeError(ReadResult::parsing_failed, "missing \"action\"");
    }

    std::string action = json.at("action").as_string().c_str();

    // Common fields for all room commands
    uint64_t room_id = 0;
    uint64_t client_id = 0;

    if (json.contains("room_id"))
    {
        room_id = Util::toUint64(json.at("room_id"));
    }

    if (json.contains("client_id"))
    {
        client_id = Util::toUint64(json.at("client_id"));
    }

    if (type == "management")
    {
        std::string room_name;
        if (json.contains("room_name"))
            room_name = json.at("room_name").as_string().c_str();

        uint64_t room_context = 0;
        if (json.contains("room_context"))
            room_context = Util::toUint64(json.at("room_context"));

        std::string username;
        if (json.contains("username"))
            username = json.at("username").as_string().c_str();

        return std::make_unique<RoomManagementCommand>(action, room_id, client_id, room_name, room_context, username);
    }
    else if (type == "player_2D")
    {
        float x = 0.0f, y = 0.0f;
        if (json.contains("x"))
        {
            x = static_cast<float>(json.at("x").as_double());
        }
        if (json.contains("y"))
        {
            y = static_cast<float>(json.at("y").as_double());
        }
        return std::make_unique<RoomPlayer2DCommand>(action, client_id, x, y);
    }
    else if (type == "player_3D")
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        uint64_t target_id = 0;
        float damage = 0.0f, new_health = 0.0f;

        if (json.contains("x"))
        {
            x = static_cast<float>(json.at("x").as_double());
        }
        if (json.contains("y"))
        {
            y = static_cast<float>(json.at("y").as_double());
        }
        if (json.contains("z"))
        {
            z = static_cast<float>(json.at("z").as_double());
        }
        if (json.contains("target_id"))
        {
            target_id = Util::toUint64(json.at("target_id"));
        }
        if (json.contains("damage"))
        {
            damage = static_cast<float>(json.at("damage").as_double());
        }
        if (json.contains("new_health"))
        {
            new_health = static_cast<float>(json.at("new_health").as_double());
        }
        return std::make_unique<RoomPlayer3DCommand>(action, client_id, x, y, z, target_id, damage, new_health);
    }
    else if (type == "object_2D")
    {
        uint64_t object_id = 0;
        float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
        std::string filepath = "";

        if (json.contains("id"))
        {
            object_id = Util::toUint64(json.at("id"));
        }
        if (json.contains("x"))
        {
            x = static_cast<float>(json.at("x").as_double());
        }
        if (json.contains("y"))
        {
            y = static_cast<float>(json.at("y").as_double());
        }
        if (json.contains("width"))
        {
            w = static_cast<float>(json.at("width").as_double());
        }
        if (json.contains("height"))
        {
            h = static_cast<float>(json.at("height").as_double());
        }
        if (json.contains("filepath"))
        {
            filepath = json.at("filepath").as_string().c_str();
        }
        return std::make_unique<RoomObject2DCommand>(action, room_id, object_id, x, y, w, h, filepath);
    }
    else if (type == "object_3D")
    {
        uint64_t object_id = 0;
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f, h = 0.0f, d = 0.0f;
        std::string filepath = "";

        if (json.contains("id"))
        {
            object_id = Util::toUint64(json.at("id"));
        }
        if (json.contains("x"))
        {
            x = static_cast<float>(json.at("x").as_double());
        }
        if (json.contains("y"))
        {
            y = static_cast<float>(json.at("y").as_double());
        }
        if (json.contains("z"))
        {
            z = static_cast<float>(json.at("z").as_double());
        }
        if (json.contains("w"))
        {
            w = static_cast<float>(json.at("w").as_double());
        }
        if (json.contains("h"))
        {
            h = static_cast<float>(json.at("h").as_double());
        }
        if (json.contains("d"))
        {
            d = static_cast<float>(json.at("d").as_double());
        }
        if (json.contains("filepath"))
        {
            filepath = json.at("filepath").as_string().c_str();
        }
        return std::make_unique<RoomObject3DCommand>(action, room_id, object_id, x, y, z, w, h, d, filepath);
    }
    else if (type == "game_item")
    {
        uint64_t item_id = 0;
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f, h = 0.0f, d = 0.0f;
        std::string item_type = "";
        std::string status = "";
        std::string filepath = "";

        if (json.contains("id"))
        {
            item_id = Util::toUint64(json.at("id"));
        }
        if (json.contains("x"))
        {
            x = static_cast<float>(json.at("x").as_double());
        }
        if (json.contains("y"))
        {
            y = static_cast<float>(json.at("y").as_double());
        }
        if (json.contains("z"))
        {
            z = static_cast<float>(json.at("z").as_double());
        }
        if (json.contains("w"))
        {
            w = static_cast<float>(json.at("w").as_double());
        }
        if (json.contains("h"))
        {
            h = static_cast<float>(json.at("h").as_double());
        }
        if (json.contains("d"))
        {
            d = static_cast<float>(json.at("d").as_double());
        }
        if (json.contains("item_type"))
        {
            item_type = json.at("item_type").as_string().c_str();
        }
        if (json.contains("status"))
        {
            status = json.at("status").as_string().c_str();
        }
        if (json.contains("filepath"))
        {
            filepath = json.at("filepath").as_string().c_str();
        }
        return std::make_unique<RoomGameItemCommand>(action, room_id, item_id, x, y, z, w, h, d, item_type, status, filepath);
    }
    else if (type == "communication")
    {
        std::string message = "";
        if (json.contains("message"))
        {
            message = json.at("message").as_string().c_str();
        }
        return std::make_unique<RoomCommunicationCommand>(action, room_id, client_id, message);
    }

    return makeError(ReadResult::parsing_failed, "cannot find room command type");
}

std::unique_ptr<BaseAPICommand> CommandParser::parseErrorCommand(std::string_view type)
{
    auto result = readResultFromString(type);
    return ErrorCommand<>::make_unique(result.value_or(ReadResult::parsing_failed));
}

} // namespace nexilis::client
