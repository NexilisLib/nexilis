/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#include <gtest/gtest.h>

#include <nexilis/client/packet.hh>
#include <nexilis/room_command_type.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/movement.hh>
#include <nexilis/server/room_storage.hh>

#include <boost/json/serialize.hpp>

using namespace nexilis;

namespace
{

constexpr uint64_t ROOM_ID = 777;
constexpr uint64_t SENDER_ID = 10;
constexpr uint64_t TARGET_ID = 20;
constexpr uint64_t OBJECT_ID = 3400;
constexpr uint64_t MESSAGE_ID = 7;
constexpr float DEFAULT_HEALTH = 100.0f;

/// Minimal server protocol that routes outbound traffic through the user's
/// boost TCP send callback.
class BoostTCPServerProtocol : public Protocol
{
public:
    void start() override
    {
    }
    void stop() override
    {
    }
    Type getType() override
    {
        return Type::BOOST_TCP_SERVER;
    }
};

/// Room command bytes: room / player_3D / shoot / targetId / damage.
nx_data shootBytes(uint64_t targetId, float damage)
{
    nx_data bytes{static_cast<uint8_t>(CommandType::room),
                  static_cast<uint8_t>(RoomCommandType::Root::player_3D),
                  static_cast<uint8_t>(RoomCommandType::PlayerType::shoot)};
    auto targetBytes = Util::convertToByteVector(targetId);
    bytes.insert(bytes.end(), targetBytes.begin(), targetBytes.end());
    auto damageBytes = Util::convertToByteVector(damage);
    bytes.insert(bytes.end(), damageBytes.begin(), damageBytes.end());
    return bytes;
}

/// Room command bytes: room / player_3D / set_team / team name.
nx_data setTeamBytes(const std::string& team)
{
    nx_data bytes{static_cast<uint8_t>(CommandType::room),
                  static_cast<uint8_t>(RoomCommandType::Root::player_3D),
                  static_cast<uint8_t>(RoomCommandType::PlayerType::set_team)};
    auto teamBytes = Util::convertToByteVector(team);
    bytes.insert(bytes.end(), teamBytes.begin(), teamBytes.end());
    return bytes;
}

/// Room command bytes: room / object_2D / create_moving / startingPosition /
/// dimensions / movement / deltaTime / movementType / filepath.
nx_data createMoving2DBytes(Vector2f startingPosition, Vector2f dimensions, Vector2f movement,
                            float deltaTime, MovementType type, const std::string& filepath)
{
    nx_data bytes{static_cast<uint8_t>(CommandType::room),
                  static_cast<uint8_t>(RoomCommandType::Root::object_2D),
                  static_cast<uint8_t>(RoomCommandType::ObjectType::create_moving)};
    auto startBytes = Util::convertToByteVector(startingPosition);
    bytes.insert(bytes.end(), startBytes.begin(), startBytes.end());
    auto dimsBytes = Util::convertToByteVector(dimensions);
    bytes.insert(bytes.end(), dimsBytes.begin(), dimsBytes.end());
    auto moveBytes = Util::convertToByteVector(movement);
    bytes.insert(bytes.end(), moveBytes.begin(), moveBytes.end());
    auto deltaBytes = Util::convertToByteVector(deltaTime);
    bytes.insert(bytes.end(), deltaBytes.begin(), deltaBytes.end());
    bytes.emplace_back(static_cast<uint8_t>(type));
    auto fileBytes = Util::convertToByteVector(filepath);
    bytes.insert(bytes.end(), fileBytes.begin(), fileBytes.end());
    return bytes;
}

/// Room command bytes: room / object_3D / create_moving / startingPosition /
/// dimensions / movement / deltaTime / movementType / filepath.
nx_data createMoving3DBytes(Vector3f startingPosition, Vector3f dimensions, Vector3f movement,
                            float deltaTime, MovementType type, const std::string& filepath)
{
    nx_data bytes{static_cast<uint8_t>(CommandType::room),
                  static_cast<uint8_t>(RoomCommandType::Root::object_3D),
                  static_cast<uint8_t>(RoomCommandType::ObjectType::create_moving)};
    auto startBytes = Util::convertToByteVector(startingPosition);
    bytes.insert(bytes.end(), startBytes.begin(), startBytes.end());
    auto dimsBytes = Util::convertToByteVector(dimensions);
    bytes.insert(bytes.end(), dimsBytes.begin(), dimsBytes.end());
    auto moveBytes = Util::convertToByteVector(movement);
    bytes.insert(bytes.end(), moveBytes.begin(), moveBytes.end());
    auto deltaBytes = Util::convertToByteVector(deltaTime);
    bytes.insert(bytes.end(), deltaBytes.begin(), deltaBytes.end());
    bytes.emplace_back(static_cast<uint8_t>(type));
    auto fileBytes = Util::convertToByteVector(filepath);
    bytes.insert(bytes.end(), fileBytes.begin(), fileBytes.end());
    return bytes;
}

/// A "getting"/"info_rooms" JSON message with a single empty room.
boost::json::object infoRoomsMessage()
{
    boost::json::object roomObj;
    roomObj["name"] = "gameplay_room";
    roomObj["max_size"] = 10;
    roomObj["room_id"] = ROOM_ID;
    roomObj["creator_id"] = SENDER_ID;
    roomObj["context"] = static_cast<uint64_t>(RoomData::Context::_3D);

    boost::json::object json;
    json["command"] = "getting";
    json["type"] = "info_rooms";
    json["callback"] = 0;
    json["rooms"] = boost::json::array{std::move(roomObj)};
    return json;
}

/// Deliver a JSON command message to the client as raw wire bytes.
client::ReadResult readClientMessage(client::ClientAPI& api, const boost::json::object& json)
{
    auto serialized = boost::json::serialize(json);
    nx_data bytes(serialized.begin(), serialized.end());
    bytes.emplace_back('\n');
    return api.readMessage(bytes);
}

/// Parse wire JSON bytes (dropping the trailing newline, if any) to an object.
boost::json::object parseMessage(const nx_data& data)
{
    nx_data jsonData = data;
    if (!jsonData.empty() && jsonData.back() == '\n')
    {
        jsonData.pop_back();
    }
    return Json::convertToJSON(jsonData);
}

class GameplaySpecTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        server::RoomStorage::clear();
        server::ClientStorage::clear();

        auto sender = std::make_unique<server::User>(SENDER_ID, "127.0.0.1");
        auto target = std::make_unique<server::User>(TARGET_ID, "127.0.0.2");
        m_sender = sender.get();
        m_target = target.get();
        server::ClientStorage::add(std::move(sender));
        server::ClientStorage::add(std::move(target));
    }

    void TearDown() override
    {
        server::RoomStorage::clear();
        server::ClientStorage::clear();
    }

    server::ServerConfig settings;
    BoostTCPServerProtocol protocol;
    server::Command command{settings};

    server::User* m_sender = nullptr;
    server::User* m_target = nullptr;

    /// Put both clients in a stored room. Every member gets a no-op outbound
    /// callback so broadcasts report success.
    void setUpRoom()
    {
        server::RoomStorage::add(server::Room(RoomData(SENDER_ID, "gameplay_room", ROOM_ID)));
        auto* room = server::RoomStorage::getRoomById(ROOM_ID);
        room->joinRoom(SENDER_ID);
        m_sender->setRoomId(ROOM_ID);
        m_sender->setBoostTCPSend([](const nx_data&) {});

        room->joinRoom(TARGET_ID);
        m_target->setRoomId(ROOM_ID);
        m_target->setBoostTCPSend([](const nx_data&) {});
    }

    server::CommandResult read(const nx_data& bytes, server::User& user)
    {
        return command.read(bytes, user, protocol, MESSAGE_ID);
    }

    /// Collect all JSON messages sent to a client via its boost TCP callback.
    std::vector<boost::json::object> receivedMessages(server::User& client)
    {
        std::vector<boost::json::object> messages;
        client.setBoostTCPSend([&messages](const nx_data& data)
                               { messages.emplace_back(parseMessage(data)); });
        return messages;
    }
};

// ---------------------------------------------------------------------------
// Shoot / damage
// ---------------------------------------------------------------------------

TEST_F(GameplaySpecTest, ShootFailsWhenNotInRoom)
{
    // The shooter has not joined any room (room id stays 0).
    EXPECT_EQ(read(shootBytes(TARGET_ID, 25.0f), *m_sender), server::CommandResult::error);
}

TEST_F(GameplaySpecTest, ShootInsufficientPayloadFails)
{
    setUpRoom();
    nx_data bytes{static_cast<uint8_t>(CommandType::room),
                  static_cast<uint8_t>(RoomCommandType::Root::player_3D),
                  static_cast<uint8_t>(RoomCommandType::PlayerType::shoot)};
    EXPECT_EQ(read(bytes, *m_sender), server::CommandResult::invalid_input);
}

TEST_F(GameplaySpecTest, ShootRoomNotFoundFails)
{
    // The shooter claims to be in a room that is not stored on the server.
    m_sender->setRoomId(ROOM_ID);
    EXPECT_EQ(read(shootBytes(TARGET_ID, 25.0f), *m_sender), server::CommandResult::failure);
}

TEST_F(GameplaySpecTest, ShootTargetNotInRoomFails)
{
    setUpRoom();
    // Only the sender is a member of the room; the target is not.
    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    room->leaveRoom(TARGET_ID);

    EXPECT_EQ(read(shootBytes(TARGET_ID, 25.0f), *m_sender), server::CommandResult::failure);
}

TEST_F(GameplaySpecTest, ShootAppliesDamageAndBroadcastsHit)
{
    setUpRoom();
    auto targetMessages = receivedMessages(*m_target);

    ASSERT_EQ(read(shootBytes(TARGET_ID, 25.0f), *m_sender), server::CommandResult::success);

    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    EXPECT_FLOAT_EQ(room->getPlayerHealth(TARGET_ID), DEFAULT_HEALTH - 25.0f);

    // A non-lethal hit never records kills/deaths or invokes the death path.
    EXPECT_EQ(room->getPlayerKills(SENDER_ID), 0);
    EXPECT_EQ(room->getPlayerDeaths(TARGET_ID), 0);

    ASSERT_EQ(targetMessages.size(), 1);
    const auto& msg = targetMessages[0];
    EXPECT_EQ(msg.at("command").as_string(), "room");
    EXPECT_EQ(msg.at("type").as_string(), "player_3D");
    EXPECT_EQ(msg.at("action").as_string(), "shoot");
    EXPECT_EQ(Util::toUint64(msg.at("room_id")), ROOM_ID);
    EXPECT_EQ(Util::toUint64(msg.at("client_id")), SENDER_ID);
    EXPECT_EQ(Util::toUint64(msg.at("target_id")), TARGET_ID);
    EXPECT_DOUBLE_EQ(msg.at("damage").as_double(), 25.0);
    EXPECT_DOUBLE_EQ(msg.at("new_health").as_double(), DEFAULT_HEALTH - 25.0);
    EXPECT_EQ(Util::toUint64(msg.at("callback")), MESSAGE_ID);
}

TEST_F(GameplaySpecTest, ShootKillTriggersDeathPathAndRecordsStats)
{
    setUpRoom();
    auto targetMessages = receivedMessages(*m_target);

    // The application's death handler is responsible for recording the kill
    // stats (the shoot command only raises the event once the hit is lethal).
    std::vector<std::pair<uint64_t, uint64_t>> deaths;
    server::RoomStorage::getRoomById(ROOM_ID)->setDeathHandler(
            [&deaths](server::Room& room, uint64_t killerId, uint64_t victimId)
            {
                room.recordKill(killerId, victimId);
                deaths.emplace_back(killerId, victimId);
            });

    ASSERT_EQ(read(shootBytes(TARGET_ID, DEFAULT_HEALTH), *m_sender), server::CommandResult::success);

    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    EXPECT_FLOAT_EQ(room->getPlayerHealth(TARGET_ID), 0.0f);
    EXPECT_EQ(room->getPlayerKills(SENDER_ID), 1);
    EXPECT_EQ(room->getPlayerDeaths(TARGET_ID), 1);

    ASSERT_EQ(deaths.size(), 1);
    EXPECT_EQ(deaths[0].first, SENDER_ID);
    EXPECT_EQ(deaths[0].second, TARGET_ID);

    // The hit broadcast reports the target at zero health.
    ASSERT_EQ(targetMessages.size(), 1);
    EXPECT_DOUBLE_EQ(targetMessages[0].at("new_health").as_double(), 0.0);
}

TEST_F(GameplaySpecTest, ShootOnDeadTargetDoesNotRecordSecondKill)
{
    setUpRoom();

    std::vector<std::pair<uint64_t, uint64_t>> deaths;
    server::RoomStorage::getRoomById(ROOM_ID)->setDeathHandler(
            [&deaths](server::Room& room, uint64_t killerId, uint64_t victimId)
            {
                room.recordKill(killerId, victimId);
                deaths.emplace_back(killerId, victimId);
            });

    ASSERT_EQ(read(shootBytes(TARGET_ID, DEFAULT_HEALTH), *m_sender), server::CommandResult::success);
    // Second shot at the already-dead (not yet respawned) target is a no-op.
    ASSERT_EQ(read(shootBytes(TARGET_ID, 50.0f), *m_sender), server::CommandResult::success);

    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    EXPECT_FLOAT_EQ(room->getPlayerHealth(TARGET_ID), 0.0f);
    EXPECT_EQ(room->getPlayerKills(SENDER_ID), 1);
    EXPECT_EQ(room->getPlayerDeaths(TARGET_ID), 1);
    EXPECT_EQ(deaths.size(), 1);
}

// ---------------------------------------------------------------------------
// Team
// ---------------------------------------------------------------------------

TEST_F(GameplaySpecTest, SetTeamNotInRoomFails)
{
    EXPECT_EQ(read(setTeamBytes("Terrorist"), *m_sender), server::CommandResult::error);
}

TEST_F(GameplaySpecTest, SetTeamUnknownTeamFails)
{
    setUpRoom();
    EXPECT_EQ(read(setTeamBytes("Blue"), *m_sender), server::CommandResult::invalid_input);
}

TEST_F(GameplaySpecTest, SetTeamRoomNotFoundFails)
{
    m_sender->setRoomId(ROOM_ID);
    EXPECT_EQ(read(setTeamBytes("Terrorist"), *m_sender), server::CommandResult::failure);
}

TEST_F(GameplaySpecTest, SetTeamValidatesTeamValueAndStoresIt)
{
    setUpRoom();
    auto* room = server::RoomStorage::getRoomById(ROOM_ID);

    ASSERT_EQ(read(setTeamBytes("Terrorist"), *m_sender), server::CommandResult::success);
    EXPECT_EQ(room->getPlayerTeam(SENDER_ID), "Terrorist");

    ASSERT_EQ(read(setTeamBytes("Counter Terrorist"), *m_sender), server::CommandResult::success);
    EXPECT_EQ(room->getPlayerTeam(SENDER_ID), "Counter Terrorist");
}

TEST_F(GameplaySpecTest, SetTeamAnnouncesLeaderboardToRoomAndSeedsJoiner)
{
    setUpRoom();
    m_sender->setUsername("shooters");

    auto senderMessages = receivedMessages(*m_sender);
    auto targetMessages = receivedMessages(*m_target);

    ASSERT_EQ(read(setTeamBytes("Terrorist"), *m_sender), server::CommandResult::success);

    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    EXPECT_EQ(room->getPlayerTeam(SENDER_ID), "Terrorist");

    // The joining player receives the room broadcast plus the seed of every
    // member's stats (the room broadcast payload first).
    ASSERT_EQ(senderMessages.size(), 2);

    const auto& broadcast = senderMessages[0];
    EXPECT_EQ(broadcast.at("command").as_string(), "room");
    EXPECT_EQ(broadcast.at("type").as_string(), "player_3D");
    EXPECT_EQ(broadcast.at("action").as_string(), "leaderboard");
    EXPECT_EQ(Util::toUint64(broadcast.at("callback")), 0);
    const auto& broadcastEntries = broadcast.at("entries").as_array();
    ASSERT_EQ(broadcastEntries.size(), 1);
    EXPECT_EQ(Util::toUint64(broadcastEntries[0].at("id")), SENDER_ID);
    EXPECT_EQ(broadcastEntries[0].at("username").as_string(), "shooters");
    EXPECT_EQ(broadcastEntries[0].at("team").as_string(), "Terrorist");
    EXPECT_EQ(Util::toUint64(broadcastEntries[0].at("kills")), 0);
    EXPECT_EQ(Util::toUint64(broadcastEntries[0].at("deaths")), 0);

    // The seed carries the joining player's entry and the other member's.
    const auto& seed = senderMessages[1];
    EXPECT_EQ(seed.at("action").as_string(), "leaderboard");
    EXPECT_EQ(Util::toUint64(seed.at("callback")), MESSAGE_ID);
    const auto& seedEntries = seed.at("entries").as_array();
    ASSERT_EQ(seedEntries.size(), 2);
    EXPECT_EQ(Util::toUint64(seedEntries[0].at("id")), SENDER_ID);
    EXPECT_EQ(seedEntries[0].at("team").as_string(), "Terrorist");
    EXPECT_EQ(Util::toUint64(seedEntries[1].at("id")), TARGET_ID);
    EXPECT_EQ(seedEntries[1].at("team").as_string(), "");

    // The other member only receives the broadcast of the joining player's stats.
    ASSERT_EQ(targetMessages.size(), 1);
    EXPECT_EQ(targetMessages[0].at("action").as_string(), "leaderboard");
    EXPECT_EQ(targetMessages[0].at("entries").as_array().size(), 1);
}

// ---------------------------------------------------------------------------
// Leaderboard helpers (server side)
// ---------------------------------------------------------------------------

TEST_F(GameplaySpecTest, PlayerStatsEntriesBuildsFullRows)
{
    setUpRoom();
    m_sender->setUsername("shooters");
    m_target->setUsername("victims");

    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    room->setPlayerTeam(SENDER_ID, "Terrorist");
    room->recordKill(SENDER_ID, TARGET_ID);
    room->recordKill(SENDER_ID, TARGET_ID);

    auto entries = server::Command::playerStatsEntries(*room, {SENDER_ID, TARGET_ID});
    ASSERT_EQ(entries.size(), 2);

    const auto& killer = entries[0].as_object();
    EXPECT_EQ(Util::toUint64(killer.at("id")), SENDER_ID);
    EXPECT_EQ(killer.at("username").as_string(), "shooters");
    EXPECT_EQ(killer.at("team").as_string(), "Terrorist");
    EXPECT_EQ(Util::toUint64(killer.at("kills")), 2);
    EXPECT_EQ(Util::toUint64(killer.at("deaths")), 0);

    const auto& victim = entries[1].as_object();
    EXPECT_EQ(Util::toUint64(victim.at("id")), TARGET_ID);
    EXPECT_EQ(victim.at("username").as_string(), "victims");
    EXPECT_EQ(victim.at("team").as_string(), "");
    EXPECT_EQ(Util::toUint64(victim.at("kills")), 0);
    EXPECT_EQ(Util::toUint64(victim.at("deaths")), 2);
}

TEST_F(GameplaySpecTest, CreateRoomLeaderboardCommandWrapsEntries)
{
    setUpRoom();

    boost::json::array entries;
    boost::json::object row;
    row["id"] = TARGET_ID;
    row["username"] = "victims";
    row["team"] = "Terrorist";
    row["kills"] = 1;
    row["deaths"] = 3;
    entries.emplace_back(std::move(row));

    auto payload = server::Command::createRoomLeaderboardCommand(ROOM_ID, *m_sender, entries, MESSAGE_ID);
    const auto json = parseMessage(payload);

    EXPECT_EQ(json.at("command").as_string(), "room");
    EXPECT_EQ(json.at("type").as_string(), "player_3D");
    EXPECT_EQ(json.at("action").as_string(), "leaderboard");
    EXPECT_EQ(Util::toUint64(json.at("callback")), MESSAGE_ID);
    EXPECT_EQ(Util::toUint64(json.at("room_id")), ROOM_ID);
    EXPECT_EQ(Util::toUint64(json.at("client_id")), SENDER_ID);

    const auto& wireEntries = json.at("entries").as_array();
    ASSERT_EQ(wireEntries.size(), 1);
    EXPECT_EQ(Util::toUint64(wireEntries[0].at("id")), TARGET_ID);
    EXPECT_EQ(wireEntries[0].at("team").as_string(), "Terrorist");
}

// ---------------------------------------------------------------------------
// Room state primitives
// ---------------------------------------------------------------------------

TEST(RoomStateTest, DamageClampsToZeroAndIgnoresDeadTarget)
{
    server::Room room(RoomData(1, "state_room", ROOM_ID));

    EXPECT_FALSE(room.damagePlayer(TARGET_ID, 30.0f));
    EXPECT_FLOAT_EQ(room.getPlayerHealth(TARGET_ID), 70.0f);

    // This hit brings the target from alive to dead.
    EXPECT_TRUE(room.damagePlayer(TARGET_ID, 70.0f));
    EXPECT_FLOAT_EQ(room.getPlayerHealth(TARGET_ID), 0.0f);

    // Dead players absorb no further damage and never re-trigger a death.
    EXPECT_FALSE(room.damagePlayer(TARGET_ID, 50.0f));
    EXPECT_FLOAT_EQ(room.getPlayerHealth(TARGET_ID), 0.0f);
}

TEST(RoomStateTest, ResetHealthRestoresDefault)
{
    server::Room room(RoomData(1, "state_room", ROOM_ID));

    ASSERT_TRUE(room.damagePlayer(TARGET_ID, DEFAULT_HEALTH));
    EXPECT_FLOAT_EQ(room.getPlayerHealth(TARGET_ID), 0.0f);

    room.resetPlayerHealth(TARGET_ID);
    EXPECT_FLOAT_EQ(room.getPlayerHealth(TARGET_ID), DEFAULT_HEALTH);
}

TEST(RoomStateTest, RecordKillUpdatesKillerAndVictimStats)
{
    server::Room room(RoomData(1, "state_room", ROOM_ID));

    room.recordKill(SENDER_ID, TARGET_ID);
    room.recordKill(SENDER_ID, TARGET_ID);

    EXPECT_EQ(room.getPlayerKills(SENDER_ID), 2);
    EXPECT_EQ(room.getPlayerDeaths(TARGET_ID), 2);
    EXPECT_EQ(room.getPlayerKills(TARGET_ID), 0);
    EXPECT_EQ(room.getPlayerDeaths(SENDER_ID), 0);
}

TEST(RoomStateTest, TeamsAreEmptyByDefaultAndStoredOnSet)
{
    server::Room room(RoomData(1, "state_room", ROOM_ID));

    EXPECT_EQ(room.getPlayerTeam(SENDER_ID), "");

    room.setPlayerTeam(SENDER_ID, "Counter Terrorist");
    EXPECT_EQ(room.getPlayerTeam(SENDER_ID), "Counter Terrorist");
}

TEST(RoomStateTest, DeathHandlerRunsOnPlayerDeath)
{
    server::Room room(RoomData(1, "state_room", ROOM_ID));

    std::vector<std::pair<uint64_t, uint64_t>> deaths;
    room.setDeathHandler([&deaths](server::Room&, uint64_t killerId, uint64_t victimId)
                         { deaths.emplace_back(killerId, victimId); });

    room.onPlayerDied(SENDER_ID, TARGET_ID);
    ASSERT_EQ(deaths.size(), 1);
    EXPECT_EQ(deaths[0].first, SENDER_ID);
    EXPECT_EQ(deaths[0].second, TARGET_ID);
}

// ---------------------------------------------------------------------------
// Moving objects (server side)
// ---------------------------------------------------------------------------

TEST_F(GameplaySpecTest, CreateMovingObject2DAddsObjectAndBroadcastsCreate)
{
    setUpRoom();
    auto targetMessages = receivedMessages(*m_target);

    const Vector2f start(10.0f, 20.0f);
    const Vector2f dims(2.0f, 3.0f);
    auto bytes = createMoving2DBytes(start, dims, Vector2f(1.0f, 0.0f), 0.001f,
                                     MovementType::eased, "sprites/ball.png");
    ASSERT_EQ(read(bytes, *m_sender), server::CommandResult::success);

    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    ASSERT_EQ(room->getObjects2D().size(), 1);
    const auto& object = room->getObjects2D()[0];
    EXPECT_FLOAT_EQ(object.getPosition().x, start.x);
    EXPECT_FLOAT_EQ(object.getPosition().y, start.y);
    EXPECT_FLOAT_EQ(object.getDimensions().x, dims.x);
    EXPECT_FLOAT_EQ(object.getDimensions().y, dims.y);
    EXPECT_EQ(object.getFilepath(), "sprites/ball.png");

    ASSERT_EQ(targetMessages.size(), 1);
    const auto& msg = targetMessages[0];
    EXPECT_EQ(msg.at("command").as_string(), "room");
    EXPECT_EQ(msg.at("type").as_string(), "object_2D");
    EXPECT_EQ(msg.at("action").as_string(), "create_moving");
    EXPECT_EQ(msg.at("createMovingType").as_string(), "create");
    EXPECT_EQ(Util::toUint64(msg.at("id")), object.getId());
    EXPECT_DOUBLE_EQ(msg.at("x").as_double(), start.x);
    EXPECT_DOUBLE_EQ(msg.at("y").as_double(), start.y);
    EXPECT_DOUBLE_EQ(msg.at("width").as_double(), dims.x);
    EXPECT_DOUBLE_EQ(msg.at("height").as_double(), dims.y);
    EXPECT_EQ(msg.at("filepath").as_string(), "sprites/ball.png");
    EXPECT_EQ(Util::toUint64(msg.at("room_id")), ROOM_ID);
    EXPECT_EQ(Util::toUint64(msg.at("client_id")), SENDER_ID);
    EXPECT_EQ(Util::toUint64(msg.at("callback")), MESSAGE_ID);

    // Give the (immediately terminating) movement thread a moment to exit
    // before TearDown releases the shared storage it reads.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST_F(GameplaySpecTest, CreateMovingObject3DAddsObjectAndBroadcastsCreate)
{
    setUpRoom();
    auto targetMessages = receivedMessages(*m_target);

    const Vector3f start(1.0f, 2.0f, 3.0f);
    const Vector3f dims(4.0f, 5.0f, 6.0f);
    auto bytes = createMoving3DBytes(start, dims, Vector3f(1.0f, 0.0f, 0.0f), 0.001f,
                                     MovementType::linear, "models/crate.obj");
    ASSERT_EQ(read(bytes, *m_sender), server::CommandResult::success);

    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    ASSERT_EQ(room->getObjects3D().size(), 1);
    const auto& object = room->getObjects3D()[0];
    EXPECT_FLOAT_EQ(object.getPosition().x, start.x);
    EXPECT_FLOAT_EQ(object.getPosition().y, start.y);
    EXPECT_FLOAT_EQ(object.getPosition().z, start.z);
    EXPECT_FLOAT_EQ(object.getDimensions().x, dims.x);
    EXPECT_FLOAT_EQ(object.getDimensions().y, dims.y);
    EXPECT_FLOAT_EQ(object.getDimensions().z, dims.z);
    EXPECT_EQ(object.getFilepath(), "models/crate.obj");

    ASSERT_EQ(targetMessages.size(), 1);
    const auto& msg = targetMessages[0];
    EXPECT_EQ(msg.at("type").as_string(), "object_3D");
    EXPECT_EQ(msg.at("action").as_string(), "create_moving");
    EXPECT_EQ(msg.at("createMovingType").as_string(), "create");
    EXPECT_EQ(Util::toUint64(msg.at("id")), object.getId());
    EXPECT_DOUBLE_EQ(msg.at("x").as_double(), start.x);
    EXPECT_DOUBLE_EQ(msg.at("y").as_double(), start.y);
    EXPECT_DOUBLE_EQ(msg.at("z").as_double(), start.z);
    EXPECT_DOUBLE_EQ(msg.at("w").as_double(), dims.x);
    EXPECT_DOUBLE_EQ(msg.at("h").as_double(), dims.y);
    EXPECT_DOUBLE_EQ(msg.at("d").as_double(), dims.z);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST_F(GameplaySpecTest, CreateMovingLinearObjectMovesOverTime)
{
    setUpRoom();
    auto targetMessages = receivedMessages(*m_target);

    const float delta = 0.25f; // 15 ticks at the default 60 Hz tickrate.
    auto bytes = createMoving2DBytes(Vector2f(0.0f, 0.0f), Vector2f(1.0f, 1.0f),
                                     Vector2f(10.0f, 0.0f), delta, MovementType::linear, "");
    ASSERT_EQ(read(bytes, *m_sender), server::CommandResult::success);

    // Wait until the movement thread has run to completion and detached.
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));

    auto* room = server::RoomStorage::getRoomById(ROOM_ID);
    ASSERT_EQ(room->getObjects2D().size(), 1);
    auto position = room->getObjects2D()[0].getPosition();

    // The object advanced along the x axis without overshooting its target.
    EXPECT_GT(position.x, 0.0f);
    EXPECT_LE(position.x, 10.0f + 0.01f);
    EXPECT_FLOAT_EQ(position.y, 0.0f);

    // The server streamed at least one update to the room members.
    bool foundUpdate = false;
    for (const auto& msg : targetMessages)
    {
        if (msg.at("createMovingType").as_string() == "update")
        {
            foundUpdate = true;
            EXPECT_TRUE(msg.contains("objectId"));
            EXPECT_GT(msg.at("x").as_double(), 0.0);
            break;
        }
    }
    EXPECT_TRUE(foundUpdate);
}

// ---------------------------------------------------------------------------
// Movement function math
// ---------------------------------------------------------------------------

TEST(MovementFunctionTest, EasingIsQuadratic)
{
    EXPECT_DOUBLE_EQ(server::Movement::easing(0.0, 100.0), 0.0);
    EXPECT_DOUBLE_EQ(server::Movement::easing(0.5, 100.0), 25.0);
    EXPECT_DOUBLE_EQ(server::Movement::easing(1.0, 100.0), 100.0);
    EXPECT_DOUBLE_EQ(server::Movement::easing(0.5, 0.0), 0.0);
}

TEST(MovementFunctionTest, LinearScalesByProgress)
{
    EXPECT_DOUBLE_EQ(server::Movement::linear(0.0, 100.0), 0.0);
    EXPECT_DOUBLE_EQ(server::Movement::linear(0.5, 100.0), 50.0);
    EXPECT_DOUBLE_EQ(server::Movement::linear(1.0, 100.0), 100.0);
}

// ---------------------------------------------------------------------------
// Client-side parsing of shoot / respawn / leaderboard / moving objects
// ---------------------------------------------------------------------------

TEST(ClientGameplayParseTest, ParsesShootIntoDamageEvent)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(SENDER_ID);

    boost::json::object json;
    json["command"] = "room";
    json["type"] = "player_3D";
    json["callback"] = 0;
    json["action"] = "shoot";
    json["room_id"] = ROOM_ID;
    json["client_id"] = SENDER_ID;
    json["target_id"] = TARGET_ID;
    json["damage"] = 25.5;
    json["new_health"] = 74.5;

    ASSERT_EQ(readClientMessage(api, json), client::ReadResult::success);

    auto events = api.consumeDamageEvents();
    ASSERT_EQ(events.size(), 1);
    EXPECT_EQ(events[0].target_id, TARGET_ID);
    EXPECT_FLOAT_EQ(events[0].damage, 25.5f);
    EXPECT_FLOAT_EQ(events[0].new_health, 74.5f);
}

TEST(ClientGameplayParseTest, ParsesRespawnIntoRespawnEvent)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(SENDER_ID);

    boost::json::object json;
    json["command"] = "room";
    json["type"] = "player_3D";
    json["callback"] = 0;
    json["action"] = "respawn";
    json["room_id"] = ROOM_ID;
    json["client_id"] = SENDER_ID;
    json["target_id"] = TARGET_ID;
    json["x"] = 10.0;
    json["y"] = 0.0;
    json["z"] = 0.0;

    ASSERT_EQ(readClientMessage(api, json), client::ReadResult::success);

    auto events = api.consumeRespawnEvents();
    ASSERT_EQ(events.size(), 1);
    EXPECT_EQ(events[0].target_id, TARGET_ID);
}

TEST(ClientGameplayParseTest, ParsesLeaderboardEntries)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(SENDER_ID);

    boost::json::object json;
    json["command"] = "room";
    json["type"] = "player_3D";
    json["callback"] = 0;
    json["action"] = "leaderboard";
    json["room_id"] = ROOM_ID;
    json["client_id"] = SENDER_ID;

    boost::json::array entries;
    boost::json::object killer;
    killer["id"] = SENDER_ID;
    killer["username"] = "shooters";
    killer["team"] = "Terrorist";
    killer["kills"] = 3;
    killer["deaths"] = 1;
    entries.emplace_back(std::move(killer));
    boost::json::object victim;
    victim["id"] = TARGET_ID;
    victim["username"] = "victims";
    victim["team"] = "Counter Terrorist";
    victim["kills"] = 1;
    victim["deaths"] = 4;
    entries.emplace_back(std::move(victim));
    json["entries"] = std::move(entries);

    ASSERT_EQ(readClientMessage(api, json), client::ReadResult::success);

    auto events = api.consumeLeaderboardEvents();
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].entries.size(), 2);

    const auto& first = events[0].entries[0];
    EXPECT_EQ(first.id, SENDER_ID);
    EXPECT_EQ(first.username, "shooters");
    EXPECT_EQ(first.team, "Terrorist");
    EXPECT_EQ(first.kills, 3);
    EXPECT_EQ(first.deaths, 1);

    const auto& second = events[0].entries[1];
    EXPECT_EQ(second.id, TARGET_ID);
    EXPECT_EQ(second.username, "victims");
    EXPECT_EQ(second.team, "Counter Terrorist");
    EXPECT_EQ(second.kills, 1);
    EXPECT_EQ(second.deaths, 4);
}

TEST(ClientGameplayParseTest, ParsesCreateMovingObject2D)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(SENDER_ID);

    ASSERT_EQ(readClientMessage(api, infoRoomsMessage()), client::ReadResult::success);

    boost::json::object json;
    json["command"] = "room";
    json["type"] = "object_2D";
    json["callback"] = 0;
    json["action"] = "create_moving";
    json["room_id"] = ROOM_ID;
    json["id"] = OBJECT_ID;
    json["x"] = 10.0;
    json["y"] = 20.0;
    json["width"] = 2.0;
    json["height"] = 3.0;
    json["filepath"] = "sprites/ball.png";

    ASSERT_EQ(readClientMessage(api, json), client::ReadResult::success);

    auto* room = api.getRoom(ROOM_ID);
    ASSERT_NE(room, nullptr);
    ASSERT_EQ(room->getObjects2D().size(), 1);
    const auto& object = room->getObjects2D()[0];
    EXPECT_EQ(object.getId(), OBJECT_ID);
    EXPECT_FLOAT_EQ(object.getPosition().x, 10.0f);
    EXPECT_FLOAT_EQ(object.getPosition().y, 20.0f);
    EXPECT_FLOAT_EQ(object.getDimensions().x, 2.0f);
    EXPECT_FLOAT_EQ(object.getDimensions().y, 3.0f);
    EXPECT_EQ(object.getFilepath(), "sprites/ball.png");
}

TEST(ClientGameplayParseTest, ParsesCreateMovingObject3D)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(SENDER_ID);

    ASSERT_EQ(readClientMessage(api, infoRoomsMessage()), client::ReadResult::success);

    boost::json::object json;
    json["command"] = "room";
    json["type"] = "object_3D";
    json["callback"] = 0;
    json["action"] = "create_moving";
    json["room_id"] = ROOM_ID;
    json["id"] = OBJECT_ID;
    json["x"] = 1.0;
    json["y"] = 2.0;
    json["z"] = 3.0;
    json["w"] = 4.0;
    json["h"] = 5.0;
    json["d"] = 6.0;
    json["filepath"] = "models/crate.obj";

    ASSERT_EQ(readClientMessage(api, json), client::ReadResult::success);

    auto snapshots = api.getRemoteObjects3DSnapshot(ROOM_ID);
    ASSERT_EQ(snapshots.size(), 1);
    EXPECT_EQ(snapshots[0].id, OBJECT_ID);
    EXPECT_FLOAT_EQ(snapshots[0].x, 1.0f);
    EXPECT_FLOAT_EQ(snapshots[0].y, 2.0f);
    EXPECT_FLOAT_EQ(snapshots[0].z, 3.0f);
    EXPECT_FLOAT_EQ(snapshots[0].w, 4.0f);
    EXPECT_FLOAT_EQ(snapshots[0].h, 5.0f);
    EXPECT_FLOAT_EQ(snapshots[0].d, 6.0f);
}

// ---------------------------------------------------------------------------
// Packet wire formats
// ---------------------------------------------------------------------------

TEST(GameplayPacketWireFormatTest, Shoot)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(1);

    auto bytes = client::Packet::Room::Player3D::shoot(api, TARGET_ID, 25.0f);
    ASSERT_EQ(bytes.size(), 16 + 3 + 8 + 4);
    EXPECT_EQ(bytes[16], static_cast<uint8_t>(CommandType::room));
    EXPECT_EQ(bytes[17], static_cast<uint8_t>(RoomCommandType::Root::player_3D));
    EXPECT_EQ(bytes[18], static_cast<uint8_t>(RoomCommandType::PlayerType::shoot));

    nx_data targetBytes(bytes.begin() + 19, bytes.begin() + 27);
    EXPECT_EQ(Util::uint64FromFront(targetBytes), TARGET_ID);
    nx_data damageBytes(bytes.begin() + 27, bytes.begin() + 31);
    EXPECT_FLOAT_EQ(Util::floatFromFront(damageBytes), 25.0f);
}

TEST(GameplayPacketWireFormatTest, SetTeam)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(1);

    auto bytes = client::Packet::Room::Player3D::setTeam(api, "Terrorist");
    ASSERT_EQ(bytes.size(), 16 + 3 + 9);
    EXPECT_EQ(bytes[16], static_cast<uint8_t>(CommandType::room));
    EXPECT_EQ(bytes[17], static_cast<uint8_t>(RoomCommandType::Root::player_3D));
    EXPECT_EQ(bytes[18], static_cast<uint8_t>(RoomCommandType::PlayerType::set_team));
    ASSERT_GE(bytes.size(), 16 + 3 + 9);
    EXPECT_EQ(bytes[19], 'T');
    EXPECT_EQ(bytes[27], 't');
}

TEST(GameplayPacketWireFormatTest, CreateMoving2D)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(1);

    const Vector2f start(1.0f, 2.0f);
    const Vector2f dims(3.0f, 4.0f);
    const Vector2f movement(5.0f, 6.0f);
    auto bytes = client::Packet::Room::Object2D::createMoving(api, start, dims, movement, 0.016f,
                                                              MovementType::eased, "a.png");
    ASSERT_EQ(bytes.size(), 16 + 3 + 8 + 8 + 8 + 4 + 1 + 5);
    EXPECT_EQ(bytes[16], static_cast<uint8_t>(CommandType::room));
    EXPECT_EQ(bytes[17], static_cast<uint8_t>(RoomCommandType::Root::object_2D));
    EXPECT_EQ(bytes[18], static_cast<uint8_t>(RoomCommandType::ObjectType::create_moving));

    // Decode the payload the same way the server command does.
    nx_data commandBytes(bytes.begin() + 16, bytes.end());
    auto payload = Util::removeAmountOfBytesFromVector(commandBytes, 3);
    auto startPos = Util::vector2fFromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 8);
    auto dimsPos = Util::vector2fFromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 8);
    auto movePos = Util::vector2fFromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 8);
    auto delta = Util::floatFromFront(payload);
    EXPECT_FLOAT_EQ(startPos.x, start.x);
    EXPECT_FLOAT_EQ(startPos.y, start.y);
    EXPECT_FLOAT_EQ(dimsPos.x, dims.x);
    EXPECT_FLOAT_EQ(dimsPos.y, dims.y);
    EXPECT_FLOAT_EQ(movePos.x, movement.x);
    EXPECT_FLOAT_EQ(movePos.y, movement.y);
    EXPECT_FLOAT_EQ(delta, 0.016f);
    EXPECT_EQ(payload[4], static_cast<uint8_t>(MovementType::eased));
}

TEST(GameplayPacketWireFormatTest, CreateMoving3D)
{
    client::ClientConfig config;
    client::ClientAPI api(config);
    api.setClientId(1);

    const Vector3f start(1.0f, 2.0f, 3.0f);
    const Vector3f dims(4.0f, 5.0f, 6.0f);
    const Vector3f movement(7.0f, 8.0f, 9.0f);
    auto bytes = client::Packet::Room::Object3D::createMoving(api, start, dims, movement, 0.033f,
                                                              MovementType::linear, "b.obj");
    ASSERT_EQ(bytes.size(), 16 + 3 + 12 + 12 + 12 + 4 + 1 + 5);
    EXPECT_EQ(bytes[16], static_cast<uint8_t>(CommandType::room));
    EXPECT_EQ(bytes[17], static_cast<uint8_t>(RoomCommandType::Root::object_3D));
    EXPECT_EQ(bytes[18], static_cast<uint8_t>(RoomCommandType::ObjectType::create_moving));

    nx_data commandBytes(bytes.begin() + 16, bytes.end());
    auto payload = Util::removeAmountOfBytesFromVector(commandBytes, 3);
    auto startPos = Util::vector3fFromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 12);
    auto dimsPos = Util::vector3fFromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 12);
    auto movePos = Util::vector3fFromFront(payload);
    payload = Util::removeAmountOfBytesFromVector(payload, 12);
    auto delta = Util::floatFromFront(payload);
    EXPECT_FLOAT_EQ(startPos.x, start.x);
    EXPECT_FLOAT_EQ(startPos.y, start.y);
    EXPECT_FLOAT_EQ(startPos.z, start.z);
    EXPECT_FLOAT_EQ(dimsPos.x, dims.x);
    EXPECT_FLOAT_EQ(dimsPos.y, dims.y);
    EXPECT_FLOAT_EQ(dimsPos.z, dims.z);
    EXPECT_FLOAT_EQ(movePos.x, movement.x);
    EXPECT_FLOAT_EQ(movePos.y, movement.y);
    EXPECT_FLOAT_EQ(movePos.z, movement.z);
    EXPECT_FLOAT_EQ(delta, 0.033f);
    EXPECT_EQ(payload[4], static_cast<uint8_t>(MovementType::linear));
}

} // namespace
