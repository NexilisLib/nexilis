#ifndef NEXILIS_CLIENT_API_HH
#define NEXILIS_CLIENT_API_HH

#include <nexilis/common/util.hh>

#include <boost/json/object.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace nexilis
{

class ClientAPI
{
public:
    class Message
    {
    public:
        Message(std::string address, std::vector<uint8_t> data)
            : m_address(address), m_data(data)
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

        /// boost UDP.
        std::string getBoostUDPServerAddress() const
        {
            return m_boostUDPServerAddress;
        }

        uint16_t getBoostUDPServerPort() const
        {
            return m_boostUDPServerPort;
        }

        void setBoostUDP(const std::string& serverAddress, uint16_t port)
        {
            m_boostUDPServerAddress = serverAddress;
            m_boostUDPServerPort = port;
        }

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

        /// boost UDP
        std::string m_boostUDPServerAddress;
        uint16_t m_boostUDPServerPort = 0xFFFF;

        /// af_unix DGRAM
        std::string m_unixDgramServerPath;

        /// af_unix STREAM
        std::string m_unixStreamServerPath;
    };

    class Room
    {
    public:
        class Client
        {
        public:
            /// Constuctor.
            Client(uint64_t id);

            /// Copy constructor.
            Client(const Client& other);

            /// Copy assignment operator.
            Client& operator=(const Client& other);

            /// Move constructor.
            Client(Client&& other);

            /// Move assignment operator.
            Client& operator=(Client&& other);

            /// Comparison operator overload.
            friend bool operator==(const Client& lhs, const Client& rhs)
            {
                return lhs.getId() == rhs.getId();
            }

            /// Non-comparison operator overload.
            friend bool operator!=(const Client& lhs, const Client& rhs)
            {
                return !(lhs == rhs);
            }

            /// Get the identifier of the client.
            uint64_t getId() const
            {
                return m_id;
            }
        private:
            uint64_t m_id;
        };

        /// Constructor.
        explicit Room(const std::string& name, uint64_t creatorId, uint64_t roomId, int maxSize, const std::vector<Client>& clients);

        /// Copy constructor.
        Room(const Room& other);

        /// Move constructor.
        Room(Room&& other);

        /// Copy assignment operator.
        Room& operator=(const Room& other);

        /// Move assignment operator.
        Room& operator=(Room&& other);

        /// Comparison operator overload.
        friend bool operator==(const Room& lhs, const Room& rhs)
        {
            return lhs.getName() == rhs.getName() &&
               lhs.getCreatorId() == rhs.getCreatorId() &&
               lhs.getRoomId() == rhs.getRoomId() &&
               lhs.getMaxSize() == rhs.getMaxSize() &&
               lhs.getClients() == rhs.getClients();
        }

        /// Non-comparison operator overload.
        friend bool operator!=(const Room& lhs, const Room& rhs)
        {
            return !(lhs == rhs);
        }

        std::string getName() const
        {
            return m_name;
        }

        uint64_t getCreatorId() const
        {
            return m_creatorId;
        }

        uint64_t getRoomId() const
        {
            return m_roomId;
        }

        int getMaxSize() const
        {
            return m_maxSize;
        }

        std::vector<Client> getClients() const
        {
            return m_clients;
        }

    private:
        std::string m_name;
        uint64_t m_creatorId;
        uint64_t m_roomId;
        int m_maxSize;
        std::vector<Client> m_clients;
    };

    /// Constructor.
    ClientAPI(ServerData data);

    /// Move constructor.
    ClientAPI(ClientAPI&& other);

    /// Move assignment operator.
    ClientAPI& operator=(ClientAPI&& other);

    /// Copy constructor.
    ClientAPI(const ClientAPI& other);

    /// Copy assignment operator.
    ClientAPI& operator=(const ClientAPI& other);

    /// Stuff related to specific connnections.
public:
    /// If the client UDP af_inet connection is ready.
    bool IsInetUDPReady();

    /// If the client TCP af_inet connection is ready.
    bool isInetTCPReady();

    /// If the client boost TCP connection is ready.
    bool isBoostTCPReady();

    /// If the client boost UDP connection is ready.
    bool isBoostUDPReady();

    /// If the client af_unix DGRAM connection is ready.
    bool isUnixDgramReady();

    /// If the client af_unix STREAM connection is ready.
    bool isUnixStreamReady();

    /// Steal the runtime until af_inet UDP connection is ready.
    void waitUntilInetUDPReady();

    /// Steal the runtime until af_inet TCP connection is ready.
    void waitUntilInetTCPReady();

    /// Steal the runtime until boost TCP connection is ready.
    void waitUntilBoostTCPReady();

    /// Steal the runtime until boost UDP connection is ready.
    void waitUntilBoostUDPReady();

    /// Steal the runtime until af_unix DGRAM connection is ready.
    void waitUntilUnixDgramReady();

    /// Steal the runtime until af_unix STREAM connection is ready.
    void waitUntilUnixStreamReady();

public:
    /// Read incoming message to client.
    bool readMessage(std::vector<uint8_t> message);

public:
    /// Is client currently in a room.
    /// \return True if the client is currently in the room.
    bool clientInRoom();

public:
    /// Getters.

    /// General.
    uint64_t getClientId() const
    {
        return m_clientId;
    }

    std::string getClientPassword() const
    {
        return m_data.getPassword();
    }

    std::string getClientUserName() const
    {
        return m_data.getUsername();
    }

    /// af_inet UDP.
    std::string getInetUDPServerAddress() const
    {
        return m_data.getInetUDPServerAddress();
    }

    uint16_t getInetUDPPortNumber() const
    {
        return m_data.getInetUDPServerPort();
    }

    /// af_inet TCP.
    std::string getInetTCPServerAddress() const
    {
        return m_data.getInetTCPServerAddress();
    }

    uint16_t getInetTCPPortNumber() const
    {
        return m_data.getInetTCPServerPort();
    }

    /// boost TCP
    std::string getBoostTCPServerAddress() const
    {
        return m_data.getBoostTCPServerAddress();
    }

    uint16_t getBoostTCPServerPortNumber() const
    {
        return m_data.getBoostTCPServerPort();
    }

    /// boost UDP
    std::string getBoostUDPServerAddress() const
    {
        return m_data.getBoostUDPServerAddress();
    }

    uint16_t getBoostUDPServerPortNumber() const
    {
        return m_data.getBoostUDPServerPort();
    }

    /// af_unix DGRAM.
    std::string getUnixDgramPath() const
    {
        return m_data.getUnixDgramServerPath();
    }

    /// af_unix STREAM.
    std::string getUnixStreamPath() const
    {
        return m_data.getUnixStreamServerPath();
    }

    /// Get the currently read received message.
    boost::json::object getCurrentMessage() const
    {
        return m_currentMessage;
    }

public:
    /// Room stuff.
    /// Return a copy of the currently active rooms.
    std::vector<Room>& getActiveRooms()
    {
        return m_currentlyActiveRooms;
    }

private:
    /// Setters.
    void setClientId(size_t id)
    {
        m_clientId = id;
    }

    /// Parse clientside data.
    bool parse(boost::json::object json);

private:
    ServerData m_data;
    uint64_t m_clientId = 0;
    boost::json::object m_currentMessage;

    std::vector<ClientAPI::Room> m_currentlyActiveRooms;
};

} // namespace nexilis

#endif
