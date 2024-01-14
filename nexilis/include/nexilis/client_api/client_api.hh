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
        ServerData(std::string afInetServerAddress, uint16_t afInetPort, const std::string& username) :
            m_AfInetServeraddress(afInetServerAddress),
            m_afInetPort(afInetPort),
            m_username(username)
        {
        }

        ServerData(std::string afInetServerAddress, uint16_t afInetPort) :
            m_AfInetServeraddress(afInetServerAddress),
            m_afInetPort(afInetPort)
        {
        }

        std::string getAfInetServerAddress() const
        {
            return m_AfInetServeraddress;
        }

        uint16_t getAfInetServerPort() const
        {
            return m_afInetPort;
        }

        std::string getUsername() const
        {
            return m_username;
        }

    private:
        // Data related specifically to server.
        std::string m_AfInetServeraddress;
        uint16_t m_afInetPort = 0xFFFF;

        // Client data in the server.
        std::string m_username;
    };

    ClientAPI(ServerData data) :
        m_data(data),
        m_af_inet_sender(data.getAfInetServerAddress().c_str(), data.getAfInetServerPort())
    {

        {
            // It would be cool if this was like a password.
            uint8_t msg[] = { 0x20, 0x10 };
            m_af_inet_sender.sendMessage(msg, sizeof(msg));
        }
    }

    bool IsAfInetUdpReady()
    {
        return  m_clientId &&
                !m_data.getAfInetServerAddress().empty() &&
                m_data.getAfInetServerPort() != 0xFFFF;
                //!m_data.client_username.empty();
    }

    void sendAfInetMessage(const std::string& message)
    {
        m_af_inet_sender.sendMessage(message);
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
        return m_data.getAfInetServerAddress();
    }

    uint16_t getServerAfInetUDPPortNumber()
    {
        return m_data.getAfInetServerPort();
    }

private:
    void initializeAfInetUdpConnection()
    {
    }

    void setClientId(size_t id)
    {
        m_clientId = &id;
    }

private:
    ServerData m_data;
    AfInetUdpSender m_af_inet_sender;
    size_t* m_clientId;
};

}

#endif
