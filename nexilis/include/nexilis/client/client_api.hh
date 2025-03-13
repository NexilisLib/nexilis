#ifndef NEXILIS_CLIENT_API_HH
#define NEXILIS_CLIENT_API_HH

#include <nexilis/base_client.hh>
#include <nexilis/base_room.hh>
#include <nexilis/json.hh>
#include <nexilis/nexilis_constants.hh>
#include <nexilis/object/object2d.hh>
#include <nexilis/object/object3d.hh>
#include <nexilis/types/vector2.hh>
#include <nexilis/types/vector3.hh>

#include <boost/json/object.hpp>

#include <cassert>
#include <cstdint>
#include <future>
#include <string>

namespace nexilis::client
{

class ClientAPI
{
public:
    class ServerData
    {
    public:
        /// Default constructor.
        ServerData() = default;

        /// Move constructor.
        ServerData(ServerData&& other);

        /// Move assignment operator.
        ServerData& operator=(ServerData&& other);

        /// Copy constructor.
        ServerData(const ServerData& other);

        /// Copy assignment operator.
        ServerData& operator=(const ServerData& other);

        const std::string& getUsername() const
        {
            return m_username;
        }

        void setUserName(const std::string& username)
        {
            m_username = username;
        }

        const std::string& getPassword() const
        {
            return m_password;
        }

        void setPassword(const std::string& password)
        {
            m_password = password;
        }

        /// af_inet UDP
        const std::string& getInetUDPServerAddress() const
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
        const std::string& getInetTCPServerAddress() const
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
        const std::string& getBoostTCPServerAddress() const
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
        const std::string& getBoostUDPServerAddress() const
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
        const std::string& getUnixDgramServerPath() const
        {
            return m_unixDgramServerPath;
        }

        void setUnixDgramServerPath(const std::string& socketPath)
        {
            m_unixDgramServerPath = socketPath;
        }

        /// af_unix STREAM
        const std::string& getUnixStreamServerPath() const
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

    class ClientSession : public BaseClient
    {
    public:
        /// Constuctor.
        ClientSession(uint64_t id, ClientAPI* clientAPI);

        /// Move constructor.
        ClientSession(ClientSession&& other);

        /// Move assignment operator.
        ClientSession& operator=(ClientSession&& other);

        /// Deleted copy constructor.
        ClientSession(const ClientSession& other) = delete;

        /// Deleted copy assignment operator.
        ClientSession& operator=(const ClientSession& other) = delete;

        /// Comparison operator overload.
        friend bool operator==(const ClientSession& lhs, const ClientSession& rhs);

        /// Non-comparison operator overload.
        friend bool operator!=(const ClientSession& lhs, const ClientSession& rhs)
        {
            return !(lhs == rhs);
        }

        void setUsername(const std::string& username)
        {
            BaseClient::setUsername(username);
        }

    private:
        /// This ClientAPI instance.
        ClientAPI* const m_clientAPI = nullptr;
    };

    class Room : public BaseRoom
    {
    public:
        /// Communication type for communications in a room.
        class Communication
        {
        public:
            /// Constructor.
            /// \param payload The data for the Communication messages.
            Communication(const std::string& payload, ClientSession* sender);

            /// Copy constructor.
            Communication(const Communication& other);

            /// Copy assignment.
            Communication& operator=(const Communication& other);

            /// Move constructor.
            Communication(Communication&& other);

            /// Move assignment operator.
            Communication& operator=(Communication&& other);

            /// Comparison operator overload.
            friend bool operator==(const Communication& lhs, const Communication& rhs);

            /// Non-comparison operator overload.
            friend bool operator!=(const Communication& lhs, const Communication& rhs)
            {
                return !(lhs == rhs);
            }

            /// Get the payload data as a string.
            const std::string& getPayload() const
            {
                return m_payload;
            }

            /// Get client identification.
            const ClientSession* getClient() const
            {
                return m_client;
            }

            uint64_t getId() const
            {
                return m_id;
            }

        private:
            /// The data of the communication.
            std::string m_payload;

            /// The id of the message.
            const ClientSession* m_client;

            /// The id of the message.
            uint64_t m_id;
        };

        /// Constructor.
        explicit Room(const RoomData& roomData, std::vector<ClientSession>&& clients);

        /// Deleted copy constructor.
        Room(const Room& other) = delete;

        /// Move constructor.
        Room(Room&& other);

        /// Deleted copy assignment operator.
        Room& operator=(const Room& other) = delete;

        /// Move assignment operator.
        Room& operator=(Room&& other);

        /// Comparison operator overload.
        friend bool operator==(const Room& lhs, const Room& rhs);

        /// Non-comparison operator overload.
        friend bool operator!=(const Room& lhs, const Room& rhs)
        {
            return !(lhs == rhs);
        }

        void addClient(ClientSession&& client)
        {
            m_clients.emplace_back(std::move(client));
        }

        void removeClient(uint64_t clientId)
        {
            m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(),
                                           [&clientId](const ClientSession& session)
                                           { return clientId == session.getId(); }),
                            m_clients.end());
        }

        std::vector<ClientSession>& getClients()
        {
            return m_clients;
        }

        const std::vector<ClientSession>& getClients() const
        {
            return m_clients;
        }

        void addMessage(Room::Communication&& broadcast)
        {
            m_roomMessages.emplace_back(std::move(broadcast));
        }

        const std::vector<Room::Communication>& getMessages() const
        {
            return m_roomMessages;
        }

        bool containsCommunication(const Room::Communication& communication);
        bool containsCommunication(uint64_t communicationId);

    private:
        /// All of the clients currently inside this room.
        std::vector<ClientSession> m_clients;

        /// All of the broadcasts that have been sent in this room.
        std::vector<Room::Communication> m_roomMessages;
    };

    /// Constructor.
    explicit ClientAPI(ServerData data);

    /// Move constructor.
    ClientAPI(ClientAPI&& other);

    /// Move assignment operator.
    ClientAPI& operator=(ClientAPI&& other);

    /// Deleted copy constructor.
    ClientAPI(const ClientAPI& other) = delete;

    /// Deleted copy assignment operator.
    ClientAPI& operator=(const ClientAPI& other) = delete;

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
    /// Result from ClientAPI::readMessage(const nx_data&).
    enum class ReadResult
    {
        // The payload does nothing with nexilis.
        clean,

        // Command success.
        success,

        // Logical failure in the command, failing is ok.
        failure,

        // Command is not found.
        not_found,

        // The input for command is not correct.
        invalid_input,

        // There is an error implementing command.
        error,

        // The command usage is unauthorized.
        unauthorized,

        // Not implemented.
        not_implemented
    };

    /// Read incoming message to client.
    ReadResult readMessage(const nx_data& message);
    void addCallback(const std::pair<uint64_t, const std::function<void()>>& callback);

public:
    /// Room stuff
    /// Is client currently in a room.
    /// \return True if the client is currently in the room.
    bool clientInRoom();

    /// The room id of the room that the client is currently in.
    uint64_t clientRoomId();

public:
    /// Getters.
    uint64_t getNewMessageId();

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

public:
    /// Room stuff.
    /// Return a reference of the currently active rooms.
    std::vector<Room>& getActiveRooms()
    {
        return m_currentlyActiveRooms;
    }

    /// Let the program wait until nexilis has created all the rooms.
    std::function<void()> waitUntilRoomsCreated(std::promise<void>& future);

    void setOverlapStatus(bool status)
    {
        m_overlappingAllowed = status;
    }

    bool overlappingAllowed() const
    {
        return m_overlappingAllowed;
    }

public:
    /// Is the server aware of the client, is the ClientAPI and Packet ready for use.
    bool isInitialized() const
    {
        return m_isInitialized;
    }

private:
    /// Setters.
    void setClientId(uint64_t id)
    {
        m_clientId = id;
    }

    /// Read the command part of the message.
    ReadResult readCommand(boost::json::object json);

    /// Read the callback part of the message.
    void readCallback(boost::json::value callback);

    std::string readString(const boost::json::value& context, const std::string& key);
    uint64_t readUint64(const boost::json::value& context, const std::string& key);
    float readFloat(const boost::json::value& context, const std::string& key);

private:
    /// The initialization data for the ClientAPI.
    ServerData m_data;

    /// The client id for the user of the client API.
    uint64_t m_clientId = 0;

    /// Rooms that client knows about.
    std::vector<ClientAPI::Room> m_currentlyActiveRooms;

    /// Existing message id's.
    std::vector<uint64_t> m_messageIds;

    /// Currently existing callbacks.
    std::vector<std::pair<uint64_t, std::function<void()>>> m_callbacks;

    /// Can the elements overlap each other.
    bool m_overlappingAllowed = false;

    /// Is the server aware of the client, is "Packet" initialized.
    bool m_isInitialized = false;
};

} // namespace nexilis::client

#endif
