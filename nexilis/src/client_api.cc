#include <nexilis/room_storage.hh>
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <nexilis/client_api.hh>
#include <nexilis/log.hh>

#include <nexilis/common/util.hh>
#include <nexilis/json.hh>
#include <nexilis/packet.hh>

#include <ostream>

namespace nexilis
{

///
/// ClientAPI::ServerData
///

ClientAPI::ServerData::ServerData(const std::string& password)
    : m_password(password)
{
}

ClientAPI::ServerData::ServerData(const std::string password, const std::string username)
    : m_password(password),
      m_username(username)
{
}

ClientAPI::ServerData::ServerData(ServerData&& other)
    : m_password(std::move(other.m_password)),
      m_username(std::move(other.m_username)),
      m_inetUDPServerAddress(std::move(other.m_inetUDPServerAddress)),
      m_inetUDPPort(std::move(other.m_inetUDPPort)),
      m_inetTCPServerAddress(std::move(other.m_inetTCPServerAddress)),
      m_inetTCPPort(std::move(other.m_inetTCPPort)),
      m_boostTCPServerAddress(std::move(other.m_boostTCPServerAddress)),
      m_boostTCPServerPort(std::move(other.m_boostTCPServerPort)),
      m_boostUDPServerAddress(std::move(other.m_boostUDPServerAddress)),
      m_boostUDPServerPort(std::move(other.m_boostUDPServerPort)),
      m_unixDgramServerPath(std::move(other.m_unixDgramServerPath)),
      m_unixStreamServerPath(std::move(other.m_unixStreamServerPath))
{
}

ClientAPI::ServerData::ServerData(const ServerData& other)
    : m_password(other.m_password),
      m_username(other.m_username),
      m_inetUDPServerAddress(other.m_inetUDPServerAddress),
      m_inetUDPPort(other.m_inetUDPPort),
      m_inetTCPServerAddress(other.m_inetTCPServerAddress),
      m_inetTCPPort(other.m_inetTCPPort),
      m_boostTCPServerAddress(other.m_boostTCPServerAddress),
      m_boostTCPServerPort(other.m_boostTCPServerPort),
      m_boostUDPServerAddress(other.m_boostUDPServerAddress),
      m_boostUDPServerPort(other.m_boostUDPServerPort),
      m_unixDgramServerPath(other.m_unixDgramServerPath),
      m_unixStreamServerPath(other.m_unixStreamServerPath)
{
}

ClientAPI::ServerData& ClientAPI::ServerData::operator=(ServerData&& other)
{
    if (this != &other)
    {
        m_password = std::move(other.m_password);
        m_username = std::move(other.m_username);
        m_inetUDPServerAddress = std::move(other.m_inetUDPServerAddress);
        m_inetUDPPort = std::move(other.m_inetUDPPort);
        m_inetTCPServerAddress = std::move(other.m_inetTCPServerAddress);
        m_inetTCPPort = std::move(other.m_inetTCPPort);
        m_boostTCPServerAddress = std::move(other.m_boostTCPServerAddress);
        m_boostTCPServerPort = std::move(other.m_boostTCPServerPort);
        m_boostUDPServerAddress = std::move(other.m_boostUDPServerAddress);
        m_boostUDPServerPort = std::move(other.m_boostUDPServerPort);
        m_unixDgramServerPath = std::move(other.m_unixDgramServerPath);
        m_unixStreamServerPath = std::move(other.m_unixStreamServerPath);
    }
    return *this;
}

ClientAPI::ServerData& ClientAPI::ServerData::operator=(const ServerData& other)
{
    if (this != &other)
    {
        m_password = other.m_password;
        m_username = other.m_username;
        m_inetUDPServerAddress = other.m_inetUDPServerAddress;
        m_inetUDPPort = other.m_inetUDPPort;
        m_inetTCPServerAddress = other.m_inetTCPServerAddress;
        m_inetTCPPort = other.m_inetTCPPort;
        m_boostTCPServerAddress = other.m_boostTCPServerAddress;
        m_boostTCPServerPort = other.m_boostTCPServerPort;
        m_boostUDPServerAddress = other.m_boostUDPServerAddress;
        m_boostUDPServerPort = other.m_boostUDPServerPort;
        m_unixDgramServerPath = other.m_unixDgramServerPath;
        m_unixStreamServerPath = other.m_unixStreamServerPath;
    }
    return *this;
}

///
/// ClientAPI
///

ClientAPI::ClientAPI(ServerData data)
    : m_data(data)
{
}

ClientAPI::ClientAPI(ClientAPI&& other)
    : m_data(std::move(other.m_data)),
      m_clientId(std::move(other.m_clientId))
{
}

ClientAPI& ClientAPI::operator=(ClientAPI&& other)
{
    if (this != &other)
    {
        m_data = std::move(other.m_data);
        m_clientId = std::move(other.m_clientId);
    }
    return *this;
}

ClientAPI::ClientAPI(const ClientAPI& other)
    : m_data(other.m_data),
      m_clientId(other.m_clientId)
{
}

ClientAPI& ClientAPI::operator=(const ClientAPI& other)
{
    if (this != &other)
    {
        m_data = other.m_data;
        m_clientId = other.m_clientId;
    }
    return *this;
}

bool ClientAPI::IsInetUDPReady()
{
    return m_clientId &&
           !m_data.getInetUDPServerAddress().empty() &&
           m_data.getInetUDPServerPort() != 0xFFFF;
}

bool ClientAPI::isInetTCPReady()
{
    return m_clientId &&
           !getInetTCPServerAddress().empty() &&
           getInetTCPPortNumber() != 0xFFFF;
}

bool ClientAPI::isBoostTCPReady()
{
    return m_clientId &&
           !getBoostTCPServerAddress().empty() &&
           getBoostTCPServerPortNumber() != 0xFFFF;
}

bool ClientAPI::isBoostUDPReady()
{
    return m_clientId &&
           !getBoostUDPServerAddress().empty() &&
           getBoostUDPServerPortNumber() != 0xFFFF;
}

bool ClientAPI::isUnixDgramReady()
{
    return m_clientId &&
           !m_data.getUnixDgramServerPath().empty();
}

bool ClientAPI::isUnixStreamReady()
{
    return m_clientId != 0 &&
           !getUnixStreamPath().empty();
}

void ClientAPI::waitUntilInetUDPReady()
{
    while (!IsInetUDPReady())
    {
    }
}

void ClientAPI::waitUntilInetTCPReady()
{
    while (!isInetTCPReady())
    {
    }
}

void ClientAPI::waitUntilBoostTCPReady()
{
    while (!isBoostTCPReady())
    {
    }
}

void ClientAPI::waitUntilBoostUDPReady()
{
    while (!isBoostUDPReady())
    {
    }
}

void ClientAPI::waitUntilUnixDgramReady()
{
    while (!isUnixDgramReady())
    {
    }
}

void ClientAPI::waitUntilUnixStreamReady()
{
    while (!isUnixStreamReady())
    {
    }
}

bool ClientAPI::parse(boost::json::object json)
{
    // Parsing message.
    if (!json.contains("nexilis_status"))
    {
        Log::info("Running code without nexilis status");
        return false;
    }
    else if (json["nexilis_status"] == 2)
    {
        Log::info("Message with nexilis_status == 2");
        return false;
    }

    if (json.contains("set_client_id"))
    {
        if (json["set_client_id"].if_uint64())
        {
            uint64_t id = json["set_client_id"].as_uint64();
            setClientId(id);
            Packet::_initialize(id);
            return true;
        }
        // boost::json::value is so bad.
        else if (json["set_client_id"].if_int64())
        {
            int64_t id = json["set_client_id"].as_int64();
            uint64_t u_id = id;

            assert(sizeof(id) == sizeof(u_id));
            assert(static_cast<uint64_t>(id) == u_id);

            setClientId(id);
            Packet::_initialize(id);
            return true;
        }
        else
        {
            Log::error("The value of set_client_id is not convertible to as_uint64");
            return false;
        }
    }
    // TODO continue parsing.
    return true;
}

bool ClientAPI::readMessage(std::vector<uint8_t> message)
{
    boost::json::object json;
    try
    {
        json = Json::convertToJSON(message);
    }
    catch (...)
    {
        Log::error("Cannot convert message to json");
        Log::debug("Trying to debug json");

        try
        {
            Json::print(json);
        }
        catch (...)
        {
        }
        return false;
    }


    if (parse(json))
    {
        return true;
    }
    else
    {
        m_currentMessage = json;
        Log::info("Received message that is not read by the server");
        return false;
    }
}

} // namespace nexilis
