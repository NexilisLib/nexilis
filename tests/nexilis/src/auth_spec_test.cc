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

#include <nexilis/auth_config.hh>
#include <nexilis/command_type.hh>
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

class MessageHandlerSkipTest : public ::testing::Test
{
protected:
    server::ServerConfig config{AuthenticationMode::skip, ""};
    server::MessageHandler handler;

    void TearDown() override
    {
        server::ClientStorage::clear();
    }

    // Build a framed command payload exactly as the skip-mode client sends it:
    // 8 byte client id (0 for a brand new client) + 8 byte message id +
    // the "getting" / "client_id" command bytes.
    std::unique_ptr<server::BaseMessage> sendHandshake()
    {
        nx_data payload;
        auto clientIdBytes = Util::convertToByteVector(static_cast<uint64_t>(0));
        auto messageIdBytes = Util::convertToByteVector(static_cast<uint64_t>(100));
        payload.insert(payload.end(), clientIdBytes.begin(), clientIdBytes.end());
        payload.insert(payload.end(), messageIdBytes.begin(), messageIdBytes.end());
        payload.emplace_back(static_cast<uint8_t>(CommandType::getting));
        payload.emplace_back(0);
        payload.emplace_back(0);
        return handler.readMessage("test_address", payload, &config);
    }
};

TEST_F(MessageHandlerSkipTest, NewClientReceivesAuthMessageWithId)
{
    auto result = sendHandshake();
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), server::BaseMessage::Type::auth_message);

    // The user must now be registered so its client id stays stable.
    auto stored = server::ClientStorage::getClientsByIpAddress("test_address");
    ASSERT_FALSE(stored.empty());
    EXPECT_NE(stored[0]->getId(), 0);
}

TEST_F(MessageHandlerSkipTest, SecondMessageFromSameClientIsHandledAsCommand)
{
    auto first = sendHandshake();
    ASSERT_NE(first, nullptr);
    ASSERT_EQ(first->getType(), server::BaseMessage::Type::auth_message);

    auto stored = server::ClientStorage::getClientsByIpAddress("test_address");
    ASSERT_FALSE(stored.empty());
    auto clientId = stored[0]->getId();

    // Send a "getting" / "client_id" command from the now-registered client
    // using its assigned id. This must be processed as a normal command.
    nx_data payload;
    auto clientIdBytes = Util::convertToByteVector(clientId);
    auto messageIdBytes = Util::convertToByteVector(static_cast<uint64_t>(200));
    payload.insert(payload.end(), clientIdBytes.begin(), clientIdBytes.end());
    payload.insert(payload.end(), messageIdBytes.begin(), messageIdBytes.end());
    payload.emplace_back(static_cast<uint8_t>(CommandType::getting));
    payload.emplace_back(0);
    payload.emplace_back(0);

    auto second = handler.readMessage("test_address", payload, &config);
    ASSERT_NE(second, nullptr);
    EXPECT_EQ(second->getType(), server::BaseMessage::Type::message);

    // The user id must not have changed between messages.
    auto again = server::ClientStorage::getClientsByIpAddress("test_address");
    ASSERT_FALSE(again.empty());
    EXPECT_EQ(again[0]->getId(), clientId);
}

} // namespace
