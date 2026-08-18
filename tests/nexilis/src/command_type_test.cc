#include <gtest/gtest.h>

#include <nexilis/command_type.hh>
#include <nexilis/room_command_type.hh>

using namespace nexilis;

TEST(CommandTypeTest, CommandTypeAsString)
{
    EXPECT_EQ(commandTypeAsString(CommandType::setting), "setting");
    EXPECT_EQ(commandTypeAsString(CommandType::getting), "getting");
    EXPECT_EQ(commandTypeAsString(CommandType::room), "room");
    EXPECT_EQ(commandTypeAsString(CommandType::authentication), "authentication");
    EXPECT_EQ(commandTypeAsString(CommandType::server_management), "server_management");
    EXPECT_EQ(commandTypeAsString(CommandType::player_management), "player_management");
    EXPECT_EQ(commandTypeAsString(CommandType::error), "error");
    EXPECT_EQ(commandTypeAsString(CommandType::undefined), "undefined");
}

TEST(CommandTypeTest, CommandTypeFromString)
{
    EXPECT_EQ(commandTypeFromString("setting"), CommandType::setting);
    EXPECT_EQ(commandTypeFromString("getting"), CommandType::getting);
    EXPECT_EQ(commandTypeFromString("room"), CommandType::room);
    EXPECT_EQ(commandTypeFromString("authentication"), CommandType::authentication);
    EXPECT_EQ(commandTypeFromString("server_management"), CommandType::server_management);
    EXPECT_EQ(commandTypeFromString("player_management"), CommandType::player_management);
    EXPECT_EQ(commandTypeFromString("error"), CommandType::error);
}

TEST(CommandTypeTest, CommandTypeFromStringUnknown)
{
    EXPECT_EQ(commandTypeFromString("unknown"), CommandType::undefined);
    EXPECT_EQ(commandTypeFromString(""), CommandType::undefined);
    EXPECT_EQ(commandTypeFromString("SETTING"), CommandType::undefined);
}

TEST(CommandTypeTest, CommandTypeRoundtrip)
{
    CommandType types[] = {CommandType::setting, CommandType::getting, CommandType::room,
                           CommandType::authentication, CommandType::server_management,
                           CommandType::player_management, CommandType::error, CommandType::undefined};

    for (auto type : types)
    {
        EXPECT_EQ(commandTypeFromString(commandTypeAsString(type)), type);
    }
}

TEST(RoomCommandTypeTest, RoomTypeToString)
{
    EXPECT_EQ(RoomCommandType::RoomTypeToString(RoomCommandType::Root::management), "management");
    EXPECT_EQ(RoomCommandType::RoomTypeToString(RoomCommandType::Root::communication), "communication");
    EXPECT_EQ(RoomCommandType::RoomTypeToString(RoomCommandType::Root::player_2D), "player_2D");
    EXPECT_EQ(RoomCommandType::RoomTypeToString(RoomCommandType::Root::object_2D), "object_2D");
    EXPECT_EQ(RoomCommandType::RoomTypeToString(RoomCommandType::Root::player_3D), "player_3D");
    EXPECT_EQ(RoomCommandType::RoomTypeToString(RoomCommandType::Root::object_3D), "object_3D");
    EXPECT_EQ(RoomCommandType::RoomTypeToString(RoomCommandType::Root::game_item), "game_item");
    EXPECT_EQ(RoomCommandType::RoomTypeToString(RoomCommandType::Root::undefined), "undefined");
}

TEST(RoomCommandTypeTest, ManagementTypeToString)
{
    EXPECT_EQ(RoomCommandType::ManagementTypeToString(RoomCommandType::Management::join), "join");
    EXPECT_EQ(RoomCommandType::ManagementTypeToString(RoomCommandType::Management::leave), "leave");
    EXPECT_EQ(RoomCommandType::ManagementTypeToString(RoomCommandType::Management::create), "create");
    EXPECT_EQ(RoomCommandType::ManagementTypeToString(RoomCommandType::Management::remove), "remove");
}

TEST(RoomCommandTypeTest, CommunicationTypeToString)
{
    EXPECT_EQ(RoomCommandType::CommunicationTypeToString(RoomCommandType::Communication::broadcast), "broadcast");
    EXPECT_EQ(RoomCommandType::CommunicationTypeToString(RoomCommandType::Communication::othercast), "othercast");
    EXPECT_EQ(RoomCommandType::CommunicationTypeToString(RoomCommandType::Communication::unicast), "unicast");
}

TEST(RoomCommandTypeTest, PlayerTypeToString)
{
    EXPECT_EQ(RoomCommandType::PlayerTypeToString(RoomCommandType::PlayerType::position), "position");
    EXPECT_EQ(RoomCommandType::PlayerTypeToString(RoomCommandType::PlayerType::dimension), "dimension");
    EXPECT_EQ(RoomCommandType::PlayerTypeToString(RoomCommandType::PlayerType::movement), "movement");
}

TEST(RoomCommandTypeTest, ObjectTypeToString)
{
    EXPECT_EQ(RoomCommandType::ObjectTypeToString(RoomCommandType::ObjectType::create), "create");
    EXPECT_EQ(RoomCommandType::ObjectTypeToString(RoomCommandType::ObjectType::destroy), "destroy");
    EXPECT_EQ(RoomCommandType::ObjectTypeToString(RoomCommandType::ObjectType::move), "move");
    EXPECT_EQ(RoomCommandType::ObjectTypeToString(RoomCommandType::ObjectType::create_moving), "create_moving");
}

TEST(RoomCommandTypeTest, GameItemActionToString)
{
    EXPECT_EQ(RoomCommandType::GameItemActionToString(RoomCommandType::GameItemAction::create), "create");
    EXPECT_EQ(RoomCommandType::GameItemActionToString(RoomCommandType::GameItemAction::update), "update");
    EXPECT_EQ(RoomCommandType::GameItemActionToString(RoomCommandType::GameItemAction::destroy), "destroy");
}
