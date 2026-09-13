#ifndef NEXILIS_SERVER_ROOM_HH
#define NEXILIS_SERVER_ROOM_HH

#include <nexilis/base_room.hh>
#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>
#include <nexilis/room_data.hh>
#include <nexilis/server/user.hh>

#include <atomic>
#include <functional>
#include <unordered_map>

namespace nexilis::server
{

/// Room objects to be stored in the RoomStorage.
class Room : public BaseRoom
{
public:
    /// Callback invoked when a player dies. The application decides what happens next
    /// (respawn, spectate, round end, etc.).
    /// \param room The room where the death occurred.
    /// \param killerId The id of the player that caused the death.
    /// \param victimId The id of the player that died.
    using DeathHandler = std::function<void(Room& room, uint64_t killerId, uint64_t victimId)>;

    /// A broadcast message that has been sent in the room.
    struct Broadcast
    {
        /// The id of the user that sent the message.
        uint64_t clientId = 0;
        /// The payload of the message.
        std::string message = "";
    };

    /// Constructor.
    explicit Room(const RoomData& roomData);

    /// Move constructor.
    Room(Room&& other);

    /// Move assignment operator.
    Room& operator=(Room&& other);

    /// Deleted copy constructor.
    Room(const Room&) = delete;

    /// Deleted copy assignment operator.
    Room& operator=(const Room&) = delete;

    /// User joins the room context.
    /// \param userId The id of the user that joins the room.
    void joinRoom(uint64_t userId);

    /// User leaves the room.
    /// \param userId The id the user that leaves the room.
    void leaveRoom(uint64_t userId);

    /// If the room contains certain user.
    /// \param userId The id of the user we are checking.
    bool contains(uint64_t userId);

    std::vector<uint64_t>& getClients()
    {
        return m_clientIds;
    }

    const std::vector<uint64_t>& getClients() const
    {
        return m_clientIds;
    }

    /// Store a broadcast message in the room.
    /// \param clientId The id of the user that sent the message.
    /// \param message The payload of the message.
    void addBroadcast(uint64_t clientId, const std::string& message);

    /// Get all broadcast messages that have been sent in the room.
    const std::vector<Broadcast>& getBroadcasts() const
    {
        return m_broadcasts;
    }

    /// Get the health of a player. Returns default health if not yet tracked.
    float getPlayerHealth(uint64_t clientId) const;

    /// Apply damage to a player. Clamps to 0.
    void damagePlayer(uint64_t clientId, float damage);

    /// Reset a player's health to default.
    void resetPlayerHealth(uint64_t clientId);

    /// Set the callback invoked when a player's health reaches zero.
    /// If no handler is set, nothing happens on death.
    void setDeathHandler(DeathHandler handler);

    /// Notify the room that a player died. Invokes the registered DeathHandler if set.
    /// \param killerId The id of the player that caused the death.
    /// \param victimId The id of the player that died.
    void onPlayerDied(uint64_t killerId, uint64_t victimId);

    /// Record a kill: increments the killer's kill count and the victim's death count.
    /// \param killerId The id of the player that caused the death.
    /// \param victimId The id of the player that died.
    void recordKill(uint64_t killerId, uint64_t victimId);

    /// Get the number of kills a player has. Returns 0 if not yet tracked.
    /// \param clientId The id of the player.
    uint64_t getPlayerKills(uint64_t clientId) const;

    /// Get the number of deaths a player has. Returns 0 if not yet tracked.
    /// \param clientId The id of the player.
    uint64_t getPlayerDeaths(uint64_t clientId) const;

    /// Set the team a player has chosen (e.g. "Terrorist"/"Counter Terrorist").
    /// \param clientId The id of the player.
    /// \param team The team name.
    void setPlayerTeam(uint64_t clientId, const std::string& team);

    /// Get the team a player has chosen. Returns empty string if unknown.
    /// \param clientId The id of the player.
    std::string getPlayerTeam(uint64_t clientId) const;

    /// Send raw data to all clients in this room.
    /// \param data The bytes to send.
    /// \return True if all clients received the data.
    bool broadcastToAll(const nx_data& data);

    /// Can clients in this room share the same position (overlap each other).
    bool isOverlappingAllowed() const
    {
        return m_overlappingAllowed.load();
    }

    /// Set whether clients in this room may share the same position.
    void setOverlappingAllowed(bool allowed)
    {
        m_overlappingAllowed.store(allowed);
    }

private:
    std::vector<uint64_t> m_clientIds;
    std::vector<Broadcast> m_broadcasts;
    std::unordered_map<uint64_t, float> m_playerHealth;
    std::unordered_map<uint64_t, uint64_t> m_playerKills;
    std::unordered_map<uint64_t, uint64_t> m_playerDeaths;
    std::unordered_map<uint64_t, std::string> m_playerTeams;
    float m_defaultHealth = 100.0f;
    DeathHandler m_deathHandler;

    /// Atomic because movement threads read this while command threads write it.
    std::atomic<bool> m_overlappingAllowed{true};
};

} // namespace nexilis::server

#endif
