#ifndef NEXILIS_CLIENT_API_HH
#define NEXILIS_CLIENT_API_HH

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

// Remember that only "common" libraries should be imported here.
// Maybe it would be clearler if these parts would be rewritten to clientside.
#include <nexilis/common/util.hh>

namespace nexilis
{

namespace af_unix
{
class UnixSocketClient;
}

class ClientAPI
{
public:
    class Message
    {
    public:
        Message(std::string address, std::vector<uint8_t> data) :
            m_address(address), m_data(data)
        {
        }

        std::string getAddress() const
        {
            return m_address;
        }

        std::vector<uint8_t> getData() const
        {
            return m_data;
        }
    private:
        std::string m_address;
        std::vector<uint8_t> m_data;
    };

    class ServerData
    {
    public:
        ServerData() = default;

        ServerData(const std::string& password) : m_password(password)
        {
        }

        ServerData(const std::string password, const std::string username) :
            m_password(password),
            m_username(username)
        {
        }

        /// af_inet UDP
        std::string getInetServerAddress() const
        {
            return m_inetUDPServeraddress;
        }

        uint16_t getInetServerPort() const
        {
            return m_inetUDPPort;
        }

        void setInetUDP(const std::string& serverAddress, uint16_t inetPort)
        {
            m_inetUDPServeraddress = serverAddress;
            m_inetUDPPort = inetPort;
        }

        /// af_unix
        std::string getUnixSocketServerPath() const
        {
            return m_unixSocketServerPath;
        }

        void setUnixSocketServerPath(const std::string& socketPath)
        {
            m_unixSocketServerPath = socketPath;
        }

        std::string getUnixSocketClientPath() const
        {
            return m_unixSocketClientPath;
        }

        void setUnixSocketClientPath(const std::string& clientPath)
        {
            m_unixSocketClientPath = clientPath;
        }

        std::string getUsername() const
        {
            return m_username;
        }

        void setUserName(const std::string username)
        {
            m_username = username;
        }

        std::string getPassword() const
        {
            return m_password;
        }

        void setPassword(const std::string& password)
        {
            m_password = password;
        }

    private:
        /// af_inet
        std::string m_inetUDPServeraddress;
        uint16_t m_inetUDPPort = 0xFFFF;

        /// af_unix
        std::string m_unixSocketServerPath;
        std::string m_unixSocketClientPath;

        /// Other client data.
        std::string m_password;
        std::string m_username;
    };

    ClientAPI(ServerData data) :
        m_data(data)
    {
    }

    bool IsInetUdpReady()
    {
        return  m_clientId &&
                !m_data.getInetServerAddress().empty() &&
                m_data.getInetServerPort() != 0xFFFF;
    }

    bool isUnixSocketClientReady()
    {
        return m_clientId &&
               !m_data.getUnixSocketServerPath().empty();
    }

    /// Read incoming message to client.
    bool readMessage(std::vector<uint8_t> message);

    std::string getServerAddress()
    {
        return m_data.getInetServerAddress();
    }

    uint16_t getServerAfInetUDPPortNumber()
    {
        return m_data.getInetServerPort();
    }

    std::string getUnixSocketServerPath()
    {
        return m_data.getUnixSocketServerPath();
    }

    std::string getUnixSocketClientPath()
    {
        return m_data.getUnixSocketClientPath();
    }

    std::string getClientPassword()
    {
        return m_data.getPassword();
    }

private:
    void setClientId(size_t id)
    {
        m_clientId = &id;
    }

private:
    ServerData m_data;
    size_t* m_clientId;

private:
    af_unix::UnixSocketClient* m_unixSocketClient;
};

}

#endif
