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
#include <nexilis/common/af_inet_udp_sender.hh>
#include <nexilis/common/util.hh>

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
        ServerData(std::string inetServerAddress, uint16_t inetPort, const std::string& username) :
            m_inetServeraddress(inetServerAddress),
            m_inetPort(inetPort),
            m_username(username)
        {
        }

        ServerData(std::string afInetServerAddress, uint16_t afInetPort) :
            m_inetServeraddress(afInetServerAddress),
            m_inetPort(afInetPort)
        {
        }

        // Yeah we need some sort a system here.
        ServerData(std::string unixSocketPath) : m_unixSocketPath(unixSocketPath){}

        /// af_inet
        std::string getInetServerAddress() const
        {
            return m_inetServeraddress;
        }

        /// af_unix
        uint16_t getInetServerPort() const
        {
            return m_inetPort;
        }

        std::string getUnixSocketPath() const
        {
            return m_unixSocketPath;
        }

        std::string getUsername() const
        {
            return m_username;
        }

    private:
        /// af_inet
        std::string m_inetServeraddress;
        uint16_t m_inetPort = 0xFFFF;

        /// af_unix
        std::string m_unixSocketPath;

        /// Other client data.
        std::string m_username;
    };

    ClientAPI(ServerData data) :
        m_data(data),
        m_inet_sender(data.getInetServerAddress().c_str(), data.getInetServerPort())
    {

        {
            // It would be cool if this was like a password.
            uint8_t msg[] = { 0x20, 0x10 };
            m_inet_sender.sendMessage(msg, sizeof(msg));
        }
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
               !m_data.getUnixSocketPath().empty();
    }

    void sendAfInetMessage(const std::string& message)
    {
        m_inet_sender.sendMessage(message);
    }

    bool readMessage(std::vector<uint8_t> message)
    {
        for (uint8_t commandByte : message)
        {
            std::cout << "Commandbyte hex: " << std::hex << static_cast<int>(commandByte);
            std::cout << std::endl;
            std::cout << "Commandbyte char: " <<  static_cast<char>(commandByte);
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
                        setClientId(id);
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
                        size_t clientId = Util::convertToType<size_t>(Util::removeAmountOfBytesFromVector(message, 1));
                        std::cout << "client id set to" << clientId << std::endl;
                        setClientId(clientId);
                        return true;
                    }

                    default: return false;
                }
            }

            default: return false;
        }

    }

    std::string getServerAddress()
    {
        return m_data.getInetServerAddress();
    }

    uint16_t getServerAfInetUDPPortNumber()
    {
        return m_data.getInetServerPort();
    }

    std::string getUnixSocketPath()
    {
        return m_data.getUnixSocketPath();
    }

private:
    void setClientId(size_t id)
    {
        m_clientId = &id;
    }

private:
    ServerData m_data;
    AfInetUdpSender m_inet_sender;
    size_t* m_clientId;
};

}

#endif
