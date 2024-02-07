#include <nexilis/client_api.hh>

namespace nexilis
{

///
/// ClientAPI::ServerData
///

ClientAPI::ServerData::ServerData(const std::string& password) : 
    m_password(password)
{
}

ClientAPI::ServerData::ServerData(const std::string password, const std::string username) :
    m_password(password),
    m_username(username)
{
}

ClientAPI::ServerData::ServerData(ServerData&& other) :
    m_password(std::move(other.m_password)),
    m_username(std::move(other.m_username)),
    m_inetUDPServerAddress(std::move(other.m_inetUDPServerAddress)),
    m_inetUDPPort(std::move(other.m_inetUDPPort)),
    m_inetTCPServerAddress(std::move(other.m_inetTCPServerAddress)),
    m_inetTCPPort(std::move(other.m_inetTCPPort)),
    m_boostTCPServerAddress(std::move(other.m_boostTCPServerAddress)),
    m_boostTCPServerPort(std::move(other.m_boostTCPServerPort)),
    m_unixDgramServerPath(std::move(other.m_unixDgramServerPath)),
    m_unixStreamServerPath(std::move(other.m_unixStreamServerPath))
{
}

ClientAPI::ServerData::ServerData(const ServerData& other) :
    m_password(other.m_password),
    m_username(other.m_username),
    m_inetUDPServerAddress(other.m_inetUDPServerAddress),
    m_inetUDPPort(other.m_inetUDPPort),
    m_inetTCPServerAddress(other.m_inetTCPServerAddress),
    m_inetTCPPort(other.m_inetTCPPort),
    m_boostTCPServerAddress(other.m_boostTCPServerAddress),
    m_boostTCPServerPort(other.m_boostTCPServerPort),
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
        m_unixDgramServerPath = other.m_unixDgramServerPath;
        m_unixStreamServerPath = other.m_unixStreamServerPath;
    }
    return *this;
}

///
/// ClientAPI
///

ClientAPI::ClientAPI(ServerData data) : 
    m_data(data)
{
}

ClientAPI::ClientAPI(ClientAPI&& other) :
    m_data(std::move(other.m_data)),
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

ClientAPI::ClientAPI(const ClientAPI& other) :
    m_data(other.m_data),
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
    return  m_clientId &&
            !m_data.getInetUDPServerAddress().empty() &&
            m_data.getInetUDPServerPort() != 0xFFFF;
}

bool ClientAPI::isInetTCPReady()
{
    return m_clientId &&
            !getInetTCPServerAddress().empty() &&
            getInetTCPPortNumber() != 0xFFFF;
}

bool ClientAPI::isUnixDgramReady()
{
    return m_clientId &&
            !m_data.getUnixDgramServerPath().empty();
}

bool ClientAPI::readMessage(std::vector<uint8_t> message)
{
    for (uint8_t commandByte : message)
    {
        std::cout << "Commandbyte hex: " << std::hex << static_cast<int>(commandByte);
        std::cout << std::endl;
        std::cout << "Commandbyte char: " <<  static_cast<char>(commandByte);
        std::cout << std::endl;
    }

    switch (message.front())
    {
        // Set
        case 0x10:
        {
            switch (message[1])
            {
                // Client ID.
                case 0x10:
                {
                    auto sizeVector = Util::removeAmountOfBytesFromVector(message, 2);
                    auto id = Util::convertToType<size_t>(sizeVector);
                    setClientId(&id);
                    return true;
                }

                default: return false;
            }

        }

        // Get.
        case 0x20:
        {
            switch (message[1])
            {
                case 0x10:
                {
                    size_t clientId = Util::convertToType<size_t>(Util::removeAmountOfBytesFromVector(message, 2));
                    std::cout << "client id set to " << clientId << std::endl;
                    setClientId(&clientId);
                    return true;
                }

                default: return false;
            }
        }

        default: return false;
    }

}

}

