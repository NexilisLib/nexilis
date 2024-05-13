#ifndef NEXILIS_CLIENT_API_HH
#define NEXILIS_CLIENT_API_HH

#include <nexilis/common/util.hh>

#include <boost/json/object.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace nexilis
{

class ClientAPI
{
public:
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
        /// Communication type for communications in a room.
        class Communication
        {
        public:
            /// Constructor.
            /// \param payload The data for the Communication messages.
            Communication(const std::string& payload, uint64_t clientId) :
                m_payload(payload),
                m_clientId(clientId)
            {
            }

            /// Copy constructor.
            Communication(const Communication& other) :
                m_payload(other.m_payload),
                m_clientId(other.m_clientId)
            {
            }

            /// Copy assignment.
            Communication& operator=(const Communication& other)
            {
                if (this != &other)
                {
                    m_payload = other.m_payload;
                    m_clientId = other.m_clientId;
                }
                return *this;
            }

            /// Move constructor.
            Communication(Communication&& other) :
                m_payload(std::move(other.m_payload)),
                m_clientId(std::move(other.m_clientId))
            {
            }

            /// Move assignment operator.
            Communication& operator=(Communication& other)
            {
                if (this != &other)
                {
                    m_payload = std::move(other.m_payload);
                    m_clientId = std::move(other.m_clientId);
                }
                return *this;
            }

            /// Get the payload data as a string.
            std::string getPayload() const
            {
                return m_payload;
            }

            /// Get client identification.
            uint64_t getClientId() const
            {
                return m_clientId;
            }
        private:
            std::string m_payload;
            uint64_t m_clientId;
        };

        /// Client type for clients in a room.
        class Client
        {
        public:
            /// Constuctor.
            Client(uint64_t id, const std::string& name = "UNDEFINED");

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

            /// Get the name if the client.
            std::string getName() const
            {
                return m_name;
            }

        private:
            /// The id of the client.
            uint64_t m_id;

            /// The name of the client.
            std::string m_name;
        };

        /// Default constructor.
        Room() = default;

        /// Constructor.
        explicit Room(const std::string& name, uint64_t creatorId, uint64_t roomId, int maxSize, const std::vector<Room::Client>& clients);

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

        std::vector<Room::Client> getClients() const
        {
            return m_clients;
        }

        void addMessage(Room::Communication&& broadcast)
        {
            m_roomMessages.emplace_back(std::move(broadcast));
        }

        std::vector<Room::Communication> getMessages()
        {
            return m_roomMessages;
        }

    private:
        /// The name of the room.
        std::string m_name;

        /// The id of the creator of this room.
        uint64_t m_creatorId;

        /// The identifier for this room.
        uint64_t m_roomId;

        /// The max amount of clients in this room.
        int m_maxSize;

        /// All clients currently inside this room.
        std::vector<Room::Client> m_clients;

        /// All of the broadcasts that have been sent in this room.
        std::vector<Room::Communication> m_roomMessages;
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
    /// Room stuff
    /// Is client currently in a room.
    /// \return True if the client is currently in the room.
    bool clientInRoom();

    /// The room id of the room that the client is currently in.
    uint64_t clientRoomId();

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

    /// Room where the client is currently in.
    Room& roomWhereClientIs(uint64_t clientId);

    ClientAPI::Room& getDefaultRoom()
    {
        return m_defaultRoom;
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
    /// The initialization data for the ClientAPI.
    ServerData m_data;

    /// The client id for the user of the client API.
    uint64_t m_clientId = 0;

    /// Last TCP message received, bad lol.
    boost::json::object m_currentMessage;

    /// Default room that as compared against.
    ClientAPI::Room m_defaultRoom;

    /// Rooms that client knows about.
    std::vector<ClientAPI::Room> m_currentlyActiveRooms;
};

} // namespace nexilis

#endif
