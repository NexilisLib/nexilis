#include <nexilis/client/client_api.hh>
#include <nexilis/client/client_session.hh>
#include <nexilis/client/command_parser.hh>
#include <nexilis/client/packet.hh>
#include <nexilis/command_type.hh>
#include <nexilis/json.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/util.hh>

#include <string>
#include <thread>

namespace nexilis::client
{

ClientAPI::ClientAPI(ServerData data)
    : NxClass("ClientAPI"),
      m_serverData(data)
{
}

ClientAPI::ClientAPI(ClientAPI&& other)
    : NxClass(std::move(other)),
      m_serverData(std::move(other.m_serverData)),
      m_clientData(std::move(other.m_clientData)),
      m_clientAPIData(std::move(other.m_clientAPIData))
{
}

ClientAPI& ClientAPI::operator=(ClientAPI&& other)
{
    if (this != &other)
    {
        m_serverData = std::move(other.m_serverData);
        m_clientData = std::move(other.m_clientData);
        m_clientAPIData = std::move(other.m_clientAPIData);

        NxClass::operator=(std::move(other));
    }
    return *this;
}

bool ClientAPI::clientInRoom()
{
    auto& rooms = getActiveRooms();

    for (auto r = rooms.begin(); r != rooms.end(); r++)
    {
        auto& clients = r->getClients();
        for (auto c = clients.begin(); c != clients.end(); c++)
        {
            if (c->getId() == getClientData().getClientId())
            {
                return true;
            }
        }
    }
    return false;
}

uint64_t ClientAPI::clientRoomId()
{
    auto& rooms = getActiveRooms();

    for (auto r = rooms.begin(); r != rooms.end(); r++)
    {
        auto& clients = r->getClients();
        for (auto c = clients.begin(); c != clients.end(); c++)
        {
            if (c->getId() == getClientData().getClientId())
            {
                return r->getId();
            }
        }
    }
    return 0;
}

uint64_t ClientAPI::getNewMessageId()
{
    uint64_t newId = Util::getRandomUint64();
    auto& messageIds = m_clientAPIData.getMessageIds();

    if (std::find(messageIds.begin(), messageIds.end(), newId) != messageIds.end())
    {
        return getNewMessageId();
    }
    else
    {
        messageIds.emplace_back(newId);
        return newId;
    }
}

void ClientAPI::addCallback(const std::pair<uint64_t, const std::function<void()>>& callback)
{
    m_clientAPIData.getCallbacks().emplace_back(callback);
}

ReadResult ClientAPI::readCommand(boost::json::object json)
{
    Log::debug(header(), "Received type: ", json["type"], " message");

    auto cmd = CommandParser::parse(json);
    return cmd->execute(*this, m_clientAPIData);
}

std::string ClientAPI::readResultStr(ReadResult res)
{
    switch (res)
    {
        case ReadResult::error:
            return "error";
        case ReadResult::parsing_failed:
            return "parsing_failed";
        case ReadResult::not_found:
            return "not_found";
        case ReadResult::success:
            return "success";
        case ReadResult::client_missing_room:
            return "client_missing_room";
        case ReadResult::failure:
            return "failure";
        case ReadResult::not_implemented:
            return "not_implemented";
        case ReadResult::unauthorized:
            return "unauthorized";
        case ReadResult::invalid_input:
            return "invalid_input";
    }
    return std::string();
}

ReadResult ClientAPI::readMessage(const nx_data& message)
{
    boost::json::object json;
    try
    {
        nx_data messageWithoutLastByte(message.begin(), message.end() - (message.empty() ? 0 : 1));
        json = Json::convertToJSON(messageWithoutLastByte);
    }
    catch (...)
    {
        Util::debugUint8Vector(message);
        return ReadResult::error;
    }

    auto result = readCommand(json);

    if (json.find("callback") != json.end() && json["callback"] != 0)
    {
        readCallback(json["callback"]);
    }

    if (result == ReadResult::success)
    {
        return result;
    }
    else
    {
        Log::error("Server returned other than \"success\"");
        Log::debug("ReadResult value: ", readResultStr(result));
        std::string stringMessage = Util::convertToString(message);
        // TODO format output json
        Log::error("Data: ", stringMessage);
    }
    return result;
}

void ClientAPI::readCallback(boost::json::value callback)
{
    uint64_t cb;
    if (callback.if_uint64())
    {
        cb = callback.as_uint64();
    }
    else if (callback.if_int64())
    {
        cb = static_cast<uint64_t>(callback.as_int64());
    }
    else
    {
        cb = 0;
    }

    // Calling a callback.
    auto& callbacks = m_clientAPIData.getCallbacks();
    for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
    {
        if (it->first == cb)
        {
            it->second();
            it = callbacks.erase(it);
            break;
        }
    }
}

std::string ClientAPI::createString(const boost::json::value& context, const std::string& key)
{
    std::string item;
    bool readGood = true;
    if (context.at(key).if_string())
    {
        item = context.at(key).as_string();
    }
    else
    {
        readGood = false;
    }
    assert(readGood);
    return item;
}

uint64_t ClientAPI::createUint64(const boost::json::value& context, const std::string& key)
{
    uint64_t item;
    bool readGood = true;
    if (context.at(key).if_uint64())
    {
        item = context.at(key).as_uint64();
    }
    else if (context.at(key).if_int64())
    {
        item = static_cast<uint64_t>(context.at(key).as_int64());
    }
    else
    {
        readGood = false;
    }
    if (!readGood)
    {
        Log::error("Could not read item with key: ", key);
        return 0;
    }
    return item;
}

float ClientAPI::createFloat(const boost::json::value& context, const std::string& key)
{
    float item = 0.f;
    bool readGood = true;
    if (context.at(key).if_double())
    {
        item = context.at(key).as_double();
    }
    else
    {
        readGood = false;
    }
    assert(readGood);
    return item;
}

std::function<void()> ClientAPI::waitUntilRoomsCreated(std::promise<void>& promise)
{
    // Capture promise by value to avoid dangling reference.
    return [promise_ptr = std::shared_ptr<std::promise<void>>(&promise, [](auto*) {}), this]()
    {
        try
        {
            // Max 5 seconds waiting time.
            constexpr int max_attempts = 50;
            int attempts = 0;

            while (attempts++ < max_attempts)
            {
                {
                    if (!m_clientAPIData.getRoomsMutex())
                    {
                        Log::error(header(), "mtx is null in waitUntilRoomsCreated");
                        break;
                    }

                    std::lock_guard<std::mutex> lock(*m_clientAPIData.getRoomsMutex());
                    if (!getActiveRooms().empty())
                    {
                        promise_ptr->set_value();
                        return;
                    }
                }
                Log::debug(header(), "Tried to getActiveRooms data!");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            // Timeout reached.
            promise_ptr->set_exception(std::make_exception_ptr(
                    std::runtime_error("Timeout waiting for rooms creation")));
        }
        catch (...)
        {
            promise_ptr->set_exception(std::current_exception());
        }
    };
}

Room* ClientAPI::getRoom(uint64_t room_id)
{
    std::lock_guard<std::mutex> lock(*m_clientAPIData.getRoomsMutex());
    for (auto& room : m_clientAPIData.getCurrentlyActiveRooms())
    {
        if (room.getId() == room_id)
        {
            return &room;
        }
    }
    return nullptr;
}

ClientSession* ClientAPI::getClientFromRoom(uint64_t client_id)
{
    ClientSession* returned_client = nullptr;
    auto& rooms = m_clientAPIData.getCurrentlyActiveRooms();
    for (auto&& room = rooms.begin(); room != rooms.end(); room++)
    {
        for (auto& client : room->getClients())
        {
            if (client.getId() == client_id)
            {
                returned_client = &client;
            }
        }
    }
    return returned_client;
}

bool ClientAPI::IsInetUDPReady()
{
    return getClientData().getClientId() && !getInetUDPServerAddress().empty();
}

bool ClientAPI::isInetTCPReady()
{
    return getClientData().getClientId() && !getInetTCPServerAddress().empty();
}

bool ClientAPI::isBoostTCPReady()
{
    return getClientData().getClientId() && !getBoostTCPServerAddress().empty();
}

bool ClientAPI::isBoostUDPReady()
{
    return getClientData().getClientId() && !getBoostUDPServerAddress().empty();
}

bool ClientAPI::isUnixDgramReady()
{
    return getClientData().getClientId() && !m_serverData.getUnixDgramServerPath().empty();
}

bool ClientAPI::isUnixStreamReady()
{
    return getClientData().getClientId() != 0 && !getUnixStreamPath().empty();
}

void ClientAPI::waitUntilInetUDPReady()
{
    while (!IsInetUDPReady())
    {
        Log::debug("ClientAPI waiting for inetUDP to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilInetTCPReady()
{
    while (!isInetTCPReady())
    {
        Log::debug("ClientAPI waiting for inetTCP to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilBoostTCPReady()
{
    while (!isBoostTCPReady())
    {
        Log::debug("ClientAPI waiting for BoostTCP to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilBoostUDPReady()
{
    while (!isBoostUDPReady())
    {
        Log::debug("ClientAPI waiting for BoostUDP to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilUnixDgramReady()
{
    while (!isUnixDgramReady())
    {
        Log::debug("ClientAPI waiting for unix dgram to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ClientAPI::waitUntilUnixStreamReady()
{
    while (!isUnixStreamReady())
    {
        Log::debug("ClientAPI waiting for unix stream to be initialized...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // namespace nexilis::client
