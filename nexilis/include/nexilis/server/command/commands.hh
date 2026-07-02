#ifndef NEXILIS_SERVER_COMMANDS_HH
#define NEXILIS_SERVER_COMMANDS_HH

#include <nexilis/command_packet_base.hh>
#include <nexilis/server/command/command_result.hh>
#include <nexilis/server/user.hh>
#include <nexilis/util.hh>

namespace nexilis::server
{

class DefaultArgs
{
public:
    DefaultArgs(User& user, Protocol& protocol, const nx_data& data, size_t messageId);

    User& getUser() const
    {
        return m_user;
    }
    Protocol& getProtocol() const
    {
        return m_protocol;
    }
    const nx_data& getData() const
    {
        return m_data;
    }
    size_t getMessageId() const
    {
        return m_messageId;
    }

    friend std::ostream& operator<<(std::ostream& os, const DefaultArgs& obj)
    {
        os << obj.getUser().getId();

        if (!obj.m_user.getUsername().empty())
        {
            os << " [" << obj.getUser().getUsername() << "]";
        }

        os << ":" << Util::convertToString(obj.getData());

        os << "\n";
        return os;
    }

private:
    User& m_user;
    Protocol& m_protocol;
    nx_data m_data;
    size_t m_messageId;
};

struct ServerImpl
{
    // Set::General
    static CommandResult set_general_clientId(const DefaultArgs& args);
    static CommandResult set_general_username(const DefaultArgs& args);

    // Set::Protocol::BoostTCP
    static CommandResult set_protocol_boosttcp_port(const DefaultArgs& args);

    // Get::General
    static CommandResult get_general_clientId(const DefaultArgs& args);
    static CommandResult get_general_roomId(const DefaultArgs& args);

    // Get::Info
    static CommandResult get_info_general(const DefaultArgs& args);
    static CommandResult get_info_clients(const DefaultArgs& args);
    static CommandResult get_info_rooms(const DefaultArgs& args);

    // Room::Management
    static CommandResult room_management_join(const DefaultArgs& args);
    static CommandResult room_management_leave(const DefaultArgs& args);
    static CommandResult room_management_create(const DefaultArgs& args);
    static CommandResult room_management_remove(const DefaultArgs& args);

    // Room::Communicate
    static CommandResult room_communicate_broadcast(const DefaultArgs& args);
    static CommandResult room_communicate_othercast(const DefaultArgs& args);
    static CommandResult room_communicate_unicast(const DefaultArgs& args);

    // Room::Player2D
    static CommandResult room_player2d_position(const DefaultArgs& args);
    static CommandResult room_player2d_dimension(const DefaultArgs& args);
    static CommandResult room_player2d_movement(const DefaultArgs& args);

    // Room::Object2D
    static CommandResult room_object2d_create(const DefaultArgs& args);
    static CommandResult room_object2d_destroy(const DefaultArgs& args);
    static CommandResult room_object2d_move(const DefaultArgs& args);
    static CommandResult room_object2d_createMoving(const DefaultArgs& args);

    // Room::Player3D
    static CommandResult room_player3d_position(const DefaultArgs& args);
    static CommandResult room_player3d_dimension(const DefaultArgs& args);
    static CommandResult room_player3d_movement(const DefaultArgs& args);

    // Room::Object3D
    static CommandResult room_object3d_create(const DefaultArgs& args);
    static CommandResult room_object3d_destroy(const DefaultArgs& args);
    static CommandResult room_object3d_move(const DefaultArgs& args);
    static CommandResult room_object3d_createMoving(const DefaultArgs& args);

    // Room::GameItem
    static CommandResult room_gameitem_create(const DefaultArgs& args);
    static CommandResult room_gameitem_update(const DefaultArgs& args);
    static CommandResult room_gameitem_destroy(const DefaultArgs& args);
};

using Commands = CommandPacketBase<ServerImpl>;

} // namespace nexilis::server

#endif
