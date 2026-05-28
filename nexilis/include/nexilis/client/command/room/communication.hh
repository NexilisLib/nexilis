#ifndef NEXILIS_CLIENT_COMMAND_ROOM_COMMUNICATION_HH
#define NEXILIS_CLIENT_COMMAND_ROOM_COMMUNICATION_HH

#include <nexilis/client/base_api_command.hh>
#include <nexilis/client/client_api.hh>

namespace nexilis::client
{

class RoomCommunicationCommand : public BaseAPICommand
{
public:
    RoomCommunicationCommand(std::string action, uint64_t room_id, uint64_t client_id, const std::string& message)
        : m_action(action), m_room_id(room_id), m_client_id(client_id), m_message(message)
    {
    }

    ReadResult execute(ClientAPI&, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        std::lock_guard<std::mutex> lock(*mtx);

        auto& rooms = data.getCurrentlyActiveRooms();

        if (m_action == "broadcast")
        {
            for (auto&& room = rooms.begin(); room != rooms.end(); room++)
            {
                ClientSession* sender = nullptr;
                for (auto& client : room->getClients())
                {
                    if (client.getId() == m_client_id)
                    {
                        sender = &client;
                    }
                }

                if (room->getId() == m_room_id)
                {
                    Room::Communication newMessage(m_message, sender);
                    auto message_id = newMessage.getId();
                    room->addMessage(std::move(newMessage));

                    assert(room->containsCommunication(message_id));
                    return ReadResult::success;
                }
            }
            return ReadResult::failure;
        }
        else if (m_action == "multicast")
        {
            // Multicast implementation would go here
            return ReadResult::not_implemented;
        }
        return ReadResult::failure;
    }

private:
    std::string m_action;
    uint64_t m_room_id;
    uint64_t m_client_id;
    std::string m_message;
};

} // namespace nexilis::client

#endif
