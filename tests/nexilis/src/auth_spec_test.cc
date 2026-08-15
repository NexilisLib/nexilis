#include <gtest/gtest.h>

#include <nexilis/auth_config.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/message/auth_message.hh>
#include <nexilis/server/message/base_message.hh>
#include <nexilis/server/message/error_message.hh>
#include <nexilis/server/message/message.hh>
#include <nexilis/server/message/message_handler.hh>
#include <nexilis/server/server_config.hh>
#include <nexilis/util.hh>

#include <string>

using namespace nexilis;

namespace
{

TEST(ConstantTimeEquals, MatchesIdenticalStrings)
{
    EXPECT_TRUE(Util::constantTimeEquals("secret", "secret"));
    EXPECT_TRUE(Util::constantTimeEquals("", ""));
}

TEST(ConstantTimeEquals, RejectsSameLengthMismatch)
{
    EXPECT_FALSE(Util::constantTimeEquals("secret", "secrex"));
}

TEST(ConstantTimeEquals, RejectsShorterString)
{
    EXPECT_FALSE(Util::constantTimeEquals("secret", "sec"));
}

TEST(ConstantTimeEquals, RejectsLongerString)
{
    EXPECT_FALSE(Util::constantTimeEquals("secret", "secretpassword"));
}

TEST(ConstantTimeEquals, RejectsLengthMismatchEvenWhenPrefixMatches)
{
    EXPECT_FALSE(Util::constantTimeEquals("secret", "secretX"));
    EXPECT_FALSE(Util::constantTimeEquals("secretX", "secret"));
}

TEST(ConstantTimeEquals, RejectsEmptyVsNonEmpty)
{
    EXPECT_FALSE(Util::constantTimeEquals("", "a"));
    EXPECT_FALSE(Util::constantTimeEquals("a", ""));
}

TEST(AuthConfig, IsPasswordMatches)
{
    AuthConfig config(AuthenticationMode::password_protected, "secret");
    EXPECT_TRUE(config.isPassword("secret"));
    EXPECT_FALSE(config.isPassword("sec"));
    EXPECT_FALSE(config.isPassword("secretpassword"));
    EXPECT_FALSE(config.isPassword("secrex"));
}

TEST(ServerConfig, IsPassphraseMatches)
{
    server::ServerConfig config;
    config.setPassphrase("secret");
    EXPECT_TRUE(config.isPassphrase("secret"));
    EXPECT_FALSE(config.isPassphrase("sec"));
    EXPECT_FALSE(config.isPassphrase("secretpassword"));
    EXPECT_FALSE(config.isPassphrase("secrex"));
}

TEST(ServerConfig, IsRootPasswordMatches)
{
    server::ServerConfig config;
    config.setRootPassword("rootsecret");
    EXPECT_TRUE(config.isRootPassword("rootsecret"));
    EXPECT_FALSE(config.isRootPassword("rootsec"));
    EXPECT_FALSE(config.isRootPassword("rootsecretpassword"));
    EXPECT_FALSE(config.isRootPassword("rootsecr"));
}

class MessageHandlerPasswordProtectedTest : public ::testing::Test
{
protected:
    server::ServerConfig config{AuthenticationMode::password_protected, "secret"};
    server::MessageHandler handler;

    void TearDown() override
    {
        server::ClientStorage::clear();
    }

    std::unique_ptr<server::BaseMessage> sendPassword(const std::string& password)
    {
        return handler.readMessage("test_address", Util::convertToByteVector(password), &config);
    }
};

TEST_F(MessageHandlerPasswordProtectedTest, CorrectShortPasswordAuthenticates)
{
    auto result = sendPassword("secret");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), server::BaseMessage::Type::auth_message);
}

TEST_F(MessageHandlerPasswordProtectedTest, WrongShortPasswordReturnsAuthError)
{
    auto result = sendPassword("abc");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), server::BaseMessage::Type::error_message);
}

TEST_F(MessageHandlerPasswordProtectedTest, WrongLongPasswordReturnsAuthError)
{
    auto result = sendPassword("a-much-longer-wrong-password");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), server::BaseMessage::Type::error_message);
}

TEST_F(MessageHandlerPasswordProtectedTest, CorrectLengthWrongPasswordReturnsAuthError)
{
    auto result = sendPassword("secrer");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), server::BaseMessage::Type::error_message);
}

TEST_F(MessageHandlerPasswordProtectedTest, EmptyPasswordReturnsAuthError)
{
    auto result = sendPassword("");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), server::BaseMessage::Type::error_message);
}

} // namespace
