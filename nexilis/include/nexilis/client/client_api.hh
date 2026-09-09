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

#include <atomic>
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
        ClientData() = default;

        ClientData(ClientData&& other) noexcept
            : m_clientId(other.m_clientId.load()),
              m_roomId(other.m_roomId.load())
        {
        }

        ClientData& operator=(ClientData&& other) noexcept
        {
            if (this != &other)
            {
                m_clientId.store(other.m_clientId.load());
                m_roomId.store(other.m_roomId.load());
            }
            return *this;
        }

        ClientData(const ClientData&) = delete;
        ClientData& operator=(const ClientData&) = delete;

        uint64_t getClientId() const
        {
            return m_clientId.load();
        };

        uint64_t getRoomId() const
        {
            return m_roomId.load();
        }

        void setClientId(uint64_t id)
        {
            m_clientId.store(id);
        }

        void setRoomId(uint64_t id)
        {
            m_roomId.store(id);
        }

    private:
        std::atomic<uint64_t> m_clientId = 0;
        std::atomic<uint64_t> m_roomId = 0;
    };

    struct DamageEvent
    {
        uint64_t target_id = 0;
        float damage = 0.0f;
        float new_health = 0.0f;
    };

    struct RespawnEvent
    {
        uint64_t target_id = 0;
    };

    class ClientAPIData
    {
    public:
        /// Constructor.
        ClientAPIData()
            : m_damageMutex(std::make_unique<std::mutex>()),
              m_roomsMutex(std::make_unique<std::mutex>())
        {
        }

        /// Move constructor.
        ClientAPIData(ClientAPIData&& other)
            : m_isInitialized(std::move(other.m_isInitialized)),
              m_isOverLappingAllowed2D(std::move(other.m_isOverLappingAllowed2D)),
              m_currentlyActiveRooms(std::move(other.m_currentlyActiveRooms)),
              m_messageIds(std::move(other.m_messageIds)),
              m_callbacks(std::move(other.m_callbacks)),
              m_pendingDamageEvents(std::move(other.m_pendingDamageEvents)),
              m_pendingRespawnEvents(std::move(other.m_pendingRespawnEvents)),
              m_damageMutex(std::move(other.m_damageMutex)),
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
                m_pendingDamageEvents = std::move(other.m_pendingDamageEvents);
                m_pendingRespawnEvents = std::move(other.m_pendingRespawnEvents);
                m_damageMutex = std::move(other.m_damageMutex);
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

        void pushDamageEvent(DamageEvent event)
        {
            std::lock_guard<std::mutex> lock(*m_damageMutex);
            m_pendingDamageEvents.push_back(std::move(event));
        }

        std::vector<DamageEvent> consumeDamageEvents()
        {
            std::lock_guard<std::mutex> lock(*m_damageMutex);
            return std::move(m_pendingDamageEvents);
        }

        void pushRespawnEvent(RespawnEvent event)
        {
            std::lock_guard<std::mutex> lock(*m_damageMutex);
            m_pendingRespawnEvents.push_back(std::move(event));
        }

        std::vector<RespawnEvent> consumeRespawnEvents()
        {
            std::lock_guard<std::mutex> lock(*m_damageMutex);
            return std::move(m_pendingRespawnEvents);
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

        /// Pending damage events.
        std::vector<DamageEvent> m_pendingDamageEvents;
        /// Pending respawn events.
        std::vector<RespawnEvent> m_pendingRespawnEvents;
        std::unique_ptr<std::mutex> m_damageMutex;

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

    /// Whether new room messages are encrypted end-to-end.
    bool isMessageEncryptionEnabled() const
    {
        return m_serverData.isMessageEncryptionEnabled();
    }

    /// Toggle end-to-end encryption of room messages on/off at runtime.
    /// \note Off by default; encryption only kicks in when enabled.
    void setMessageEncryption(bool enabled)
    {
        m_serverData.setMessageEncryption(enabled);
    }

    /// Whether the TCP connection is encrypted with TLS-PSK.
    bool isTlsEnabled() const
    {
        return m_serverData.isTlsEnabled();
    }

    /// Toggle TLS-PSK transport encryption on/off.
    /// \note Off by default; the transport upgrade applies when the client
    ///       connects, so set it before start().
    void setTls(bool enabled)
    {
        m_serverData.setTls(enabled);
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

    /// Consume all pending damage events (thread-safe).
    std::vector<DamageEvent> consumeDamageEvents()
    {
        return m_clientAPIData.consumeDamageEvents();
    }

    /// Consume all pending respawn events (thread-safe).
    std::vector<RespawnEvent> consumeRespawnEvents()
    {
        return m_clientAPIData.consumeRespawnEvents();
    }

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
