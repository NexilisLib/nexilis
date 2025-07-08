#ifndef NEXILIS_CLIENT_API_HH
#define NEXILIS_CLIENT_API_HH

#include <nexilis/logger/file_log.hh>
#include <nexilis/logger/log.hh>

#include <nexilis/client/room.hh>
#include <nexilis/client/server_data.hh>
#include <nexilis/nx_class.hh>

#include <nexilis/json.hh>
#include <nexilis/nexilis_constants.hh>
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
        /// The payload has nothing with nexilis.
        clean,

        /// Command success.
        success,

        /// Allowed failure.
        failure,

        /// Something wrong with the command data.
        not_found,

        /// The input for command is not correct.
        invalid_input,

        /// Internal error.
        error,

        /// The command usage is unauthorized.
        unauthorized,

        /// Client is not found in the correct room.
        client_missing_room,

        /// Missing feature.
        not_implemented
    };

    /// Get ReadResult string value.
    /// \param res The ReadResult enum from "readMessage"
    static std::string readResultStr(ReadResult res);

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
        assert(m_isInitialized);
        return m_clientId;
    }

    std::string getClientPassword() const
    {
        return m_data.getPassword();
    }

    std::string getClientUsername(uint64_t client_id)
    {
        auto* client = getClientFromRoom(client_id);
        return client ? client->getUsername() : "";
    }

    /// af_inet UDP.
    std::string getInetUDPServerAddress() const
    {
        return m_data.getInetUDPServerAddress();
    }

    /// af_inet TCP.
    std::string getInetTCPServerAddress() const
    {
        return m_data.getInetTCPServerAddress();
    }

    /// boost TCP
    std::string getBoostTCPServerAddress() const
    {
        return m_data.getBoostTCPServerAddress();
    }

    uint16_t getBoostTCPServerPortNumber() const
    {
        return m_data.getBoostTCPServerPortNumber();
    }

    void setBoostTCPPortNumber(uint16_t port)
    {
        m_data.setBoostTCPPortNumber(port);
    }

    /// boost UDP
    std::string getBoostUDPServerAddress() const
    {
        return m_data.getBoostUDPServerAddress();
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

    /// Get a reference to a room from room id.
    Room* getRoom(uint64_t room_id);

    /// Get client pointer from any room.
    ClientSession* getClientFromRoom(uint64_t client_id);

    /// Let the program wait until nexilis has created all the rooms.
    std::function<void()> waitUntilRoomsCreated(std::promise<void>& future);

    /// Set the value of 2D overlapping.
    void setOverlapStatus(bool status)
    {
        m_2DoverlappingAllowed = status;
    }

    /// Get the value of 2D overlapping.
    bool overlappingAllowed2D() const
    {
        return m_2DoverlappingAllowed;
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
    std::vector<client::Room> m_currentlyActiveRooms;

    /// Existing message id's.
    std::vector<uint64_t> m_messageIds;

    /// Currently existing callbacks.
    std::vector<std::pair<uint64_t, std::function<void()>>> m_callbacks;

    /// Can the 2D elements overlap each other.
    bool m_2DoverlappingAllowed = false;

    /// Is the server aware of the client, is "Packet" initialized.
    bool m_isInitialized = false;
};

} // namespace nexilis::client

#endif
