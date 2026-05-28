#include <nexilis/client/command/room/communication.hh>
#include <nexilis/client/command/room/management.hh>
#include <nexilis/client/command/room/object2d.hh>
#include <nexilis/client/command/room/player2d.hh>
#include <nexilis/client/command/room/player3d.hh>
#include <nexilis/client/command/set/username.hh>
#include <nexilis/client/command_parser.hh>

namespace nexilis::client
{

std::unique_ptr<BaseAPICommand> CommandParser::parseSettingCommand(const boost::json::object& json, std::string_view type)
{
    if (type == "username")
    {
        std::string username = json.at("username").as_string().c_str();
        return std::make_unique<SetUsernameCommand>(username);
    }
    else if (type == "port")
    {
        uint16_t port = static_cast<uint16_t>(json.at("boost_tcp_port").as_uint64());
        // For port setting, we would need to create a separate command class
        return nullptr;
    }
    return nullptr;
}

std::unique_ptr<BaseAPICommand> CommandParser::parseGettingCommand(const boost::json::object& json, std::string_view type)
{
    // For getting commands, we don't currently have implementations as they're handled directly
    // in the ClientAPI::readCommand method
    return nullptr;
}

std::unique_ptr<BaseAPICommand> CommandParser::parseRoomCommand(const boost::json::object& json, std::string_view type)
{
    // Parse room commands based on 'type' and 'action' fields
    if (!json.contains("action"))
    {
        return nullptr;
    }

    std::string action = json.at("action").as_string().c_str();

    // Common fields for all room commands
    uint64_t room_id = 0;
    uint64_t client_id = 0;

    if (json.contains("roomId"))
    {
        room_id = json.at("roomId").as_uint64();
    }

    if (json.contains("clientId"))
    {
        client_id = json.at("clientId").as_uint64();
    }

    if (type == "management")
    {
        return std::make_unique<RoomManagementCommand>(action, room_id, client_id);
    }
    else if (type == "player2D")
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
    else if (type == "player3D")
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;
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
        return std::make_unique<RoomPlayer3DCommand>(action, client_id, x, y, z);
    }
    else if (type == "object2D")
    {
        uint64_t object_id = 0;
        float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
        std::string filepath = "";

        if (json.contains("id"))
        {
            object_id = json.at("id").as_uint64();
        }
        if (json.contains("positionX"))
        {
            x = static_cast<float>(json.at("positionX").as_double());
        }
        if (json.contains("positionY"))
        {
            y = static_cast<float>(json.at("positionY").as_double());
        }
        if (json.contains("dimensionX"))
        {
            w = static_cast<float>(json.at("dimensionX").as_double());
        }
        if (json.contains("dimensionY"))
        {
            h = static_cast<float>(json.at("dimensionY").as_double());
        }
        if (json.contains("filepath"))
        {
            filepath = json.at("filepath").as_string().c_str();
        }
        return std::make_unique<RoomObject2DCommand>(action, room_id, object_id, x, y, w, h, filepath);
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

    return nullptr;
}

} // namespace nexilis::client