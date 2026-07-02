#include <nexilis/client/room.hh>

namespace nexilis::client
{

Room::Communication::Communication(const std::string& payload, ClientSession* client)
    : m_payload(payload),
      m_client(client),
      m_id(Util::getRandomUint64())
{
}

Room::Communication::Communication(const Communication& other)
    : m_payload(other.m_payload),
      m_client(other.m_client),
      m_id(other.m_id)
{
}

Room::Communication& Room::Communication::operator=(const Communication& other)
{
    if (this != &other)
    {
        m_payload = other.m_payload;
        m_client = other.m_client;
        m_id = other.m_id;
    }
    return *this;
}

Room::Communication::Communication(Communication&& other)
    : m_payload(std::move(other.m_payload)),
      m_client(std::move(other.m_client)),
      m_id(std::move(other.m_id))

{
}

Room::Communication& Room::Communication::operator=(Communication&& other)
{
    if (this != &other)
    {
        m_payload = std::move(other.m_payload);
        m_client = std::move(other.m_client);
        m_id = std::move(other.m_id);
    }
    return *this;
}

bool operator==(const Room::Communication& lhs, const Room::Communication& rhs)
{
    return lhs.getPayload() == rhs.getPayload() &&
           lhs.getClient() == rhs.getClient() &&
           lhs.getId() == rhs.getId();
}

Room::Room(Room&& other)
    : BaseRoom(std::move(other)),
      m_clients(std::move(other.m_clients)),
      m_roomMessages(std::move(other.m_roomMessages))
{
}

Room& Room::operator=(Room&& other)
{
    if (this != &other)
    {
        static_cast<BaseRoom&>(*this) = static_cast<BaseRoom&&>(other);
        m_clients = std::move(other.m_clients);
        m_roomMessages = std::move(other.m_roomMessages);
    }
    return *this;
}

Room::Room(const RoomData& roomData, std::vector<ClientSession>&& clients)
    : BaseRoom(roomData),
      m_clients(std::move(clients))
{
}

bool operator==(const Room& lhs, const Room& rhs)
{
    return lhs.getClients() == rhs.getClients() &&
           lhs.getMessages() == rhs.getMessages();
}

bool Room::containsCommunication(const Room::Communication& communication)
{
    return std::find(m_roomMessages.begin(), m_roomMessages.end(), communication) != m_roomMessages.end();
}

bool Room::containsCommunication(uint64_t communicationId)
{
    auto idComparator = [communicationId](const Communication& item)
    {
        return item.getId() == communicationId;
    };
    return std::find_if(m_roomMessages.begin(), m_roomMessages.end(), idComparator) != m_roomMessages.end();
}

} // namespace nexilis::client
