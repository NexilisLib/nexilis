#include <gtest/gtest.h>

#include <nexilis/client/client_config.hh>
#include <nexilis/crypto.hh>
#include <nexilis/server/server_config.hh>
#include <nexilis/tls.hh>

#include <string>

using namespace nexilis;

namespace
{

TEST(TLS, ClientConfigOffByDefault)
{
    client::ClientConfig config;
    EXPECT_FALSE(config.isTlsEnabled());
}

TEST(TLS, ClientConfigToggleFlipsState)
{
    client::ClientConfig config;
    config.setTls(true);
    EXPECT_TRUE(config.isTlsEnabled());
    config.setTls(false);
    EXPECT_FALSE(config.isTlsEnabled());
}

TEST(TLS, ServerConfigOffByDefault)
{
    server::ServerConfig config;
    EXPECT_FALSE(config.isTlsEnabled());
}

TEST(TLS, ServerConfigToggleFlipsState)
{
    server::ServerConfig config;
    config.setTls(true);
    EXPECT_TRUE(config.isTlsEnabled());
    config.setTls(false);
    EXPECT_FALSE(config.isTlsEnabled());
}

TEST(TLS, DerivePskIsDeterministic)
{
    const auto first = tls::derivePsk("salasana");
    const auto second = tls::derivePsk("salasana");
    EXPECT_EQ(first, second);
    EXPECT_FALSE(first.empty());
}

TEST(TLS, DerivePskKeyLength)
{
    EXPECT_EQ(tls::derivePsk("salasana").size(), tls::kPskLength);
    EXPECT_EQ(tls::kPskLength, 32u);
}

TEST(TLS, DerivePskDiffersBetweenPasswords)
{
    EXPECT_NE(tls::derivePsk("salasana"), tls::derivePsk("salasama"));
}

TEST(TLS, DerivePskDiffersFromMessageKey)
{
    const auto psk = tls::derivePsk("salasana");
    const auto messageKey = crypto::deriveMessageKey("salasana", 42);
    EXPECT_NE(psk, messageKey);
}

TEST(TLS, CreatePskContextFailsWithoutPassword)
{
    EXPECT_EQ(tls::createPskContext("", false), nullptr);
    EXPECT_EQ(tls::createPskContext("", true), nullptr);
}

TEST(TLS, CreatePskContextServer)
{
    auto ctx = tls::createPskContext("salasana", true);
    EXPECT_NE(ctx, nullptr);
}

TEST(TLS, CreatePskContextClient)
{
    auto ctx = tls::createPskContext("salasana", false);
    EXPECT_NE(ctx, nullptr);
}

} // namespace