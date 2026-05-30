#include <gtest/gtest.h>

#include <nexilis/client/packet.hh>
#include <nexilis/command_spec.hh>
#include <nexilis/server/client_storage.hh>

#include <cstring>

using namespace nexilis;

class CommandSpecTest : public ::testing::Test
{
protected:
    CommandSpec spec;

    void SetUp() override
    {
        auto userPtr = std::make_unique<server::User>(spec.user.getId(), spec.user.getIPAddress());
        server::ClientStorage::add(std::move(userPtr));
    }

    void TearDown() override
    {
        server::ClientStorage::clear();
    }
};

TEST_F(CommandSpecTest, Constructs)
{
    EXPECT_EQ(spec.user.getId(), 1);
    EXPECT_EQ(spec.user.getUsername(), "");
    EXPECT_EQ(spec.protocol.getType(), Protocol::Type::UNKNOWN);
}

TEST_F(CommandSpecTest, ArgsHelper)
{
    const nx_data data = {0x00, 0x00, 0x01, 'a'};
    auto da = spec.args(data, 7);

    EXPECT_EQ(da.getUser().getId(), 1);
    EXPECT_EQ(da.getMessageId(), 7);
    EXPECT_EQ(da.getData().size(), data.size());
    EXPECT_EQ(da.getData()[3], 'a');
}

TEST_F(CommandSpecTest, PacketWireFormat)
{
    auto bytes = client::Packet::Set::General::username("abc");

    ASSERT_GE(bytes.size(), 6);
    EXPECT_EQ(bytes[0], static_cast<uint8_t>(CommandType::setting));
    EXPECT_EQ(bytes[1], 0);
    EXPECT_EQ(bytes[2], 1);
    EXPECT_EQ(bytes[3], 'a');
    EXPECT_EQ(bytes[4], 'b');
    EXPECT_EQ(bytes[5], 'c');
}

TEST_F(CommandSpecTest, SetUsernameFromRawBytes)
{
    nx_data bytes = {0x00, 0x00, 0x01};
    const char* name = "test_user";
    bytes.insert(bytes.end(), name, name + std::strlen(name));

    EXPECT_EQ(spec.read(bytes), server::CommandResult::success);
}

TEST_F(CommandSpecTest, SetUsernameViaPacketApi)
{
    auto bytes = client::Packet::Set::General::username("packet_user");
    EXPECT_EQ(spec.read(bytes), server::CommandResult::success);
}

TEST_F(CommandSpecTest, SetUsernameMultipleTimes)
{
    for (int i = 0; i < 3; i++)
    {
        nx_data bytes = {0x00, 0x00, 0x01};
        bytes.push_back('a' + i);
        EXPECT_EQ(spec.read(bytes), server::CommandResult::success);
    }
}

TEST_F(CommandSpecTest, SetClientIdFailsWithoutRoot)
{
    nx_data bytes = {0x00, 0x00, 0x00, 0x2A, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00};
    EXPECT_EQ(spec.read(bytes), server::CommandResult::unauthorized);
}

TEST_F(CommandSpecTest, UndefinedCommandTypeReturnsError)
{
    // CommandType::undefined (8) explicitly returns CommandResult::error.
    nx_data bytes = {0x08, 0x00, 0x00, 0x00};
    EXPECT_EQ(spec.read(bytes), server::CommandResult::error);
}

TEST_F(CommandSpecTest, PacketAndCommandsAreDifferentTypes)
{
    EXPECT_FALSE((std::is_same_v<client::Packet, server::Commands>));
}

TEST_F(CommandSpecTest, ServerCommandResultEnum)
{
    EXPECT_TRUE(server::checkResult(server::CommandResult::success));
    EXPECT_FALSE(server::checkResult(server::CommandResult::failure));
    EXPECT_FALSE(server::checkResult(server::CommandResult::error));
}
