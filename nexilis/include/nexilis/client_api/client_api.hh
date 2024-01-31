#ifndef NEXILIS_CLIENT_API_HH
#define NEXILIS_CLIENT_API_HH

#include <nexilis/common/util.hh>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

namespace nexilis
{

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

        /// af_inet UDP
        std::string getInetUDPServerAddress() const
        {
            return m_inetUDPServerAddress;
        }

        uint16_t getInetUDPServerPort() const
        {
            return m_inetUDPPort;
        }

        void setInetUDP(const std::string& serverAddress, uint16_t port)
        {
            m_inetUDPServerAddress = serverAddress;
            m_inetUDPPort = port;
        }

        /// af_inet TCP
        std::string getInetTCPServerAddress() const
        {
            return m_inetTCPServerAddress;
        }

        uint16_t getInetTCPServerPort() const
        {
            return m_inetTCPPort;
        }

        void setInetTCP(const std::string& serverAddress, uint16_t port)
        {
            m_inetTCPServerAddress = serverAddress;
            m_inetTCPPort = port;
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

    private:
        /// Client data.
        std::string m_password;
        std::string m_username;

        /// af_inet UDP
        std::string m_inetUDPServerAddress;
        uint16_t m_inetUDPPort = 0xFFFF;

        /// af_inet TCP
        std::string m_inetTCPServerAddress;
        uint16_t m_inetTCPPort = 0xFFFF;

        /// af_unix
        // TODO separation between sock_stream and sock_dgram.
        std::string m_unixSocketServerPath;
    };

    ClientAPI(ServerData data) :
        m_data(data)
    {
    }

    bool IsInetUDPReady()
    {
        return  m_clientId &&
                !m_data.getInetUDPServerAddress().empty() &&
                m_data.getInetUDPServerPort() != 0xFFFF;
    }

    bool isInetTCPReady()
    {
        return m_clientId &&
               !getInetTCPServerAddress().empty() &&
               getInetTCPPortNumber() != 0xFFFF;
    }

    bool isUnixSocketClientReady()
    {
        return m_clientId &&
               !m_data.getUnixSocketServerPath().empty();
    }

    /// Read incoming message to client.
    bool readMessage(std::vector<uint8_t> message);

    std::string getInetUDPServerAddress()
    {
        return m_data.getInetUDPServerAddress();
    }

    uint16_t getInetUDPPortNumber()
    {
        return m_data.getInetUDPServerPort();
    }

    std::string getInetTCPServerAddress()
    {
        return m_data.getInetTCPServerAddress();
    }

    uint16_t getInetTCPPortNumber()
    {
        return m_data.getInetTCPServerPort();
    }

    std::string getUnixSocketServerPath()
    {
        return m_data.getUnixSocketServerPath();
    }

    std::string getClientPassword()
    {
        return m_data.getPassword();
    }

private:
    void setClientId(size_t* id)
    {
        m_clientId = id;
    }

private:
    ServerData m_data;
    size_t* m_clientId;
};

}

#endif
