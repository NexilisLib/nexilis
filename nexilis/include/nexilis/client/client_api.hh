#ifndef NEXILIS_CLIENT_API_HH
#define NEXILIS_CLIENT_API_HH

#include <nexilis/logger/file_log.hh>
#include <nexilis/logger/log.hh>

#include <nexilis/client/client_config.hh>
#include <nexilis/client/read_result.hh>
#include <nexilis/client/room.hh>
#include <nexilis/nx_class.hh>

#include <nexilis/json.hh>
#include <nexilis/nexilis_constants.hh>
#include <nexilis/object/game_item.hh>
#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>
#include <nexilis/types/vector2.hh>
#include <nexilis/types/vector3.hh>

#include <boost/json/object.hpp>

#include <cassert>
#include <cstdint>
#include <future>
#include <string>

namespace nexilis::client
{

class ClientAPI : public NxClass
{
public:
    class ClientData
    {
    public:
        uint64_t getClientId() const
        {
            return m_clientId;
        };

        uint64_t getRoomId() const
        {
            return m_roomId;
        }

        void setClientId(uint64_t id)
        {
            m_clientId = id;
        }

        void setRoomId(uint64_t id)
        {
            m_roomId = id;
        }

    private:
        uint64_t m_clientId = 0;
        uint64_t m_roomId = 0;
    };

    class ClientAPIData
    {
    public:
        /// Constructor.
        ClientAPIData()
            : m_roomsMutex(std::make_unique<std::mutex>())
        {
        }

        /// Move constructor.
        ClientAPIData(ClientAPIData&& other)
            : m_isInitialized(std::move(other.m_isInitialized)),
              m_isOverLappingAllowed2D(std::move(other.m_isOverLappingAllowed2D)),
              m_currentlyActiveRooms(std::move(other.m_currentlyActiveRooms)),
              m_messageIds(std::move(other.m_messageIds)),
              m_callbacks(std::move(other.m_callbacks)),
              m_roomsMutex(std::move(other.m_roomsMutex))
        {
            if (!other.m_roomsMutex)
            {
                other.m_roomsMutex = std::make_unique<std::mutex>();
            }
        }

        /// Move assignment operator.
        ClientAPIData& operator=(ClientAPIData&& other)
        {
            if (this != &other)
            {
                m_isInitialized = std::move(other.m_isInitialized);
                m_isOverLappingAllowed2D = std::move(other.m_isOverLappingAllowed2D);
                m_currentlyActiveRooms = std::move(other.m_currentlyActiveRooms);
                m_messageIds = std::move(other.m_messageIds);
                m_callbacks = std::move(other.m_callbacks);
                m_roomsMutex = std::move(other.m_roomsMutex);

                if (!other.m_roomsMutex)
                {
                    other.m_roomsMutex = std::make_unique<std::mutex>();
                }
            }
            return *this;
        }

        /// Deleted copy constructor.
        ClientAPIData(const ClientAPIData& other) = delete;

        /// Deleted copy assignment operator.
        ClientAPIData& operator=(const ClientAPIData& other) = delete;

        void initialize()
        {
            m_isInitialized = true;
        }

        bool isInitialized() const
        {
            return m_isInitialized;
        }

        bool isOverlappingAllowed2D() const
        {
            return m_isOverLappingAllowed2D;
        }

        void setOverlapStatus2D(bool status)
        {
            m_isOverLappingAllowed2D = status;
        }

        const std::vector<client::Room>& getCurrentlyActiveRooms() const
        {
            return m_currentlyActiveRooms;
        }

        std::vector<client::Room>& getCurrentlyActiveRooms()
        {
            return m_currentlyActiveRooms;
        }

        void setCurrentlyActiveRooms(std::vector<client::Room>&& rooms)
        {
            m_currentlyActiveRooms = std::move(rooms);
        }

        const std::vector<uint64_t>& getMessageIds() const
        {
            return m_messageIds;
        }

        std::vector<uint64_t>& getMessageIds()
        {
            return m_messageIds;
        }

        std::vector<std::pair<uint64_t, std::function<void()>>>& getCallbacks()
        {
            return m_callbacks;
        }

        std::unique_ptr<std::mutex>& getRoomsMutex()
        {
            return m_roomsMutex;
        }

    private:
        /// Is the server aware of the client, is "Packet" initialized.
        bool m_isInitialized = false;

        /// Can the 2D elements overlap each other.
        bool m_isOverLappingAllowed2D = false;

        /// Rooms that the client knows about.
        std::vector<client::Room> m_currentlyActiveRooms;

        /// Existing message id's.
        std::vector<uint64_t> m_messageIds;

        /// Currently existing callbacks.
        std::vector<std::pair<uint64_t, std::function<void()>>> m_callbacks;

        // Mutex for room operations.
        std::unique_ptr<std::mutex> m_roomsMutex;
    };

    /// Constructor.
    explicit ClientAPI(ClientConfig data);

    /// Move constructor.
    ClientAPI(ClientAPI&& other);

    /// Move assignment operator.
    ClientAPI& operator=(ClientAPI&& other);

    /// Deleted copy constructor.
    ClientAPI(const ClientAPI& other) = delete;

    /// Deleted copy assignment operator.
    ClientAPI& operator=(const ClientAPI& other) = delete;

    /// Get ReadResult string value.
    /// \param res The ReadResult enum from "readMessage"
    static std::string readResultStr(ReadResult res);

    /// Read incoming message to client.
    ReadResult readMessage(const nx_data& message);
    void addCallback(const std::pair<uint64_t, const std::function<void()>>& callback);

    /// Is the server aware of the client, is the ClientAPI and Packet ready for use.
    bool isInitialized() const
    {
        return m_clientAPIData.isInitialized();
    }

    /// Room stuff
    /// Is client currently in a room.
    /// \return True if the client is currently in the room.
    bool clientInRoom();

    /// The room id of the room that the client is currently in.
    uint64_t clientRoomId();

    uint64_t getNewMessageId();

    void setClientId(uint64_t id)
    {
        m_clientData.setClientId(id);
    }

    void setRoomId(uint64_t id)
    {
        m_clientData.setRoomId(id);
    }

    /// General.
    uint64_t getClientId() const
    {
        return m_clientData.getClientId();
    }

    server::AuthenticationMode getMode() const
    {
        return m_serverData.getMode();
    }

    std::string getClientPassword() const
    {
        return m_serverData.getPassword();
    }

    std::string getClientUsername(uint64_t client_id)
    {
        auto* client = getClientFromRoom(client_id);
        return client ? client->getUsername() : "";
    }

    /// Return a reference of the currently active rooms.
    const std::vector<client::Room>& getActiveRooms() const
    {
        return m_clientAPIData.getCurrentlyActiveRooms();
    }
    std::vector<client::Room>& getActiveRooms()
    {
        return m_clientAPIData.getCurrentlyActiveRooms();
    }

    /// Get a reference to a room from room id.
    Room* getRoom(uint64_t room_id);

    /// Get client pointer from any room.
    ClientSession* getClientFromRoom(uint64_t client_id);

    /// Get a copy of a client's 3D position.
    Vector3f getClientPosition3D(uint64_t client_id);

    struct RemotePlayerSnapshot
    {
        uint64_t id = 0;
        float x = 0.0f, y = 0.0f, z = 0.0f;
        float w = 0.0f, h = 0.0f, d = 0.0f;
    };

    struct RemoteObject3DSnapshot
    {
        uint64_t id = 0;
        float x = 0.0f, y = 0.0f, z = 0.0f;
        float w = 0.0f, h = 0.0f, d = 0.0f;
    };

    struct RemoteGameItemSnapshot
    {
        uint64_t id = 0;
        std::string item_type;
        float x = 0.0f, y = 0.0f, z = 0.0f;
        float w = 0.0f, h = 0.0f, d = 0.0f;
        std::string status;
        std::string filepath;
    };

    std::vector<RemoteObject3DSnapshot> getRemoteObjects3DSnapshot(uint64_t room_id);

    std::vector<RemoteGameItemSnapshot> getRemoteGameItemsSnapshot(uint64_t room_id);

    /// Get a snapshot of all remote players in a room.
    /// \param room_id The room to query.
    /// \param my_id The local client id to exclude from results.
    std::vector<RemotePlayerSnapshot> getRemotePlayersSnapshot(uint64_t room_id, uint64_t my_id);

    /// Let the program wait until nexilis has created all the rooms.
    std::function<void()> waitUntilRoomsCreated(std::promise<void>& future, const uint16_t max_attempts = 50, const uint16_t timeout = 100);

    /// Set the value of 2D overlapping.
    void setOverLapStatus2D(bool status)
    {
        m_clientAPIData.setOverlapStatus2D(status);
    }

    /// Get the value of 2D overlapping.
    bool isOverlappingAllowed2D() const
    {
        return m_clientAPIData.isOverlappingAllowed2D();
    }

    /// Stuff related to specific connnections.

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

    /// af_inet UDP.
    std::string getInetUDPServerAddress() const
    {
        return m_serverData.getInetUDPServerAddress();
    }

    /// af_inet TCP.
    std::string getInetTCPServerAddress() const
    {
        return m_serverData.getInetTCPServerAddress();
    }

    /// boost TCP
    std::string getBoostTCPServerAddress() const
    {
        return m_serverData.getBoostTCPServerAddress();
    }

    uint16_t getBoostTCPServerPortNumber() const
    {
        return m_serverData.getBoostTCPServerPortNumber();
    }

    void setBoostTCPPortNumber(uint16_t port)
    {
        m_serverData.setBoostTCPPortNumber(port);
    }

    void setProtocolPort(const std::string& protocol, uint16_t port)
    {
        m_serverData.setProtocolPort(protocol, port);
    }

    /// boost UDP
    std::string getBoostUDPServerAddress() const
    {
        return m_serverData.getBoostUDPServerAddress();
    }

    /// af_unix DGRAM.
    std::string getUnixDgramPath() const
    {
        return m_serverData.getUnixDgramServerPath();
    }

    /// af_unix STREAM.
    std::string getUnixStreamPath() const
    {
        return m_serverData.getUnixStreamServerPath();
    }

private:
    /// Setters.
    ClientData& getClientData()
    {
        return m_clientData;
    }

    /// Read the command part of the message.
    ReadResult readCommand(boost::json::object json);

    /// Read the callback part of the message.
    void readCallback(boost::json::value callback);

private:
    /// The initialization data for the ClientAPI.
    ClientConfig m_serverData;

    ClientData m_clientData;

    ClientAPIData m_clientAPIData;
};

} // namespace nexilis::client

#endif
