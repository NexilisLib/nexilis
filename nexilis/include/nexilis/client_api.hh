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
        /// Default constructor.
        ServerData() = default;

        /// Constructor.
        /// \param password The password matching "commonPassword" in the server code.
        ServerData(const std::string& password);

        /// Constructor.
        /// \param password The password matching "commonPassword" in the server code.
        ServerData(const std::string password, const std::string username);

        /// Move constructor.
        ServerData(ServerData&& other);

        /// Move assignment operator.
        ServerData& operator=(ServerData&& other);

        /// Copy constructor.
        ServerData(const ServerData& other);

        /// Copy assignment operator.
        ServerData& operator=(const ServerData& other);

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

        /// boost TCP.
        std::string getBoostTCPServerAddress() const
        {
            return m_boostTCPServerAddress;
        }

        uint16_t getBoostTCPServerPort() const
        {
            return m_boostTCPServerPort;
        }

        void setBoostTCP(const std::string& serverAddress, uint16_t port)
        {
            m_boostTCPServerAddress = serverAddress;
            m_boostTCPServerPort = port;
        }

        /// TODO boost UDP.

        /// af_unix DGRAM
        std::string getUnixDgramServerPath() const
        {
            return m_unixDgramServerPath;
        }

        void setUnixDgramServerPath(const std::string& socketPath)
        {
            m_unixDgramServerPath = socketPath;
        }

        /// af_unix STREAM
        std::string getUnixStreamServerPath() const
        {
            return m_unixStreamServerPath;
        }

        void setUnixStreamServerPath(const std::string& socketPath)
        {
            m_unixStreamServerPath = socketPath;
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

        /// boost TCP
        std::string m_boostTCPServerAddress;
        uint16_t m_boostTCPServerPort = 0xFFFF;

        /// boost UDP TODO.

        /// af_unix DGRAM
        std::string m_unixDgramServerPath;

        /// af_unix STREAM
        std::string m_unixStreamServerPath;
    };

    class Command
    {
    public:
        class Get
        {
        public:
            static std::vector<uint8_t> clientId(ClientAPI& api)
            {
                std::cout << "CLIENTID WHEN SENDING" << api.m_clientId << std::endl;
                std::vector<uint8_t> clientIdVector = Util::convertToByteVector(api.m_clientId);
                clientIdVector.push_back(0xFF);
                clientIdVector.push_back(0x20);
                clientIdVector.push_back(0x10);
                return clientIdVector;
            }
        };
    };

    /// Constructor.
    ClientAPI(ServerData data);

    /// Move constructor.
    ClientAPI(ClientAPI&& other);

    /// Move assignment operator.
    ClientAPI& operator=(ClientAPI&& other);

    /// Deleted copy constructor.
    ClientAPI(const ClientAPI& other);

    /// Deleted copy assignment.
    ClientAPI& operator=(const ClientAPI& other);

    bool IsInetUDPReady();

    bool isInetTCPReady();

    bool isUnixDgramReady();

    /// Read incoming message to client.
    bool readMessage(std::vector<uint8_t> message);

public:
    /// Getters.
    std::string getClientPassword()
    {
        return m_data.getPassword();
    }

    std::string getClientUserName()
    {
        return m_data.getUsername();
    }

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

    std::string getUnixDgramPath()
    {
        return m_data.getUnixDgramServerPath();
    }

    std::string getUnixStreamPath()
    {
        return m_data.getUnixStreamServerPath();
    }

    /// Setters.
private:
    void setClientId(size_t id)
    {
        m_clientId = id;
    }

private:
    ServerData m_data;
    size_t m_clientId;
};

}

#endif
