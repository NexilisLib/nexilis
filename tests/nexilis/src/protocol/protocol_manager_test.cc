#include <gtest/gtest.h>

#include <nexilis/protocol.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server/server_config.hh>

#include <nexilis/server/protocol/af_unix/stream_server.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>

class ProtocolManagerTest : public ::testing::Test
{
protected:
    nexilis::ProtocolManager manager;
};

TEST_F(ProtocolManagerTest, CreateProtocol_UnixStreamServer)
{
    nexilis::server::ServerConfig settings;
    settings.setMode(nexilis::server::AuthenticationMode::password_protected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    auto unix_stream_server = manager.createProtocol<nexilis::server::af_unix::StreamServer>(settings, "/tmp/nexilis/stream");

    EXPECT_EQ(unix_stream_server.getType(), nexilis::Protocol::Type::AF_UNIX_SOCK_STREAM_SERVER);
    EXPECT_EQ(unix_stream_server.getSettings().getMode(), nexilis::server::AuthenticationMode::password_protected);
    EXPECT_EQ(unix_stream_server.getSettings().getPassphrase(), "salasana");
    EXPECT_EQ(unix_stream_server.getSettings().getRootPassword(), "root");
}

TEST_F(ProtocolManagerTest, CreateProtocol_BoostTCPServer)
{
    nexilis::server::ServerConfig settings;
    settings.setMode(nexilis::server::AuthenticationMode::password_protected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    auto boost_tcp_server = manager.createProtocol<nexilis::server::nxboost::TCPServer>(settings);

    EXPECT_EQ(boost_tcp_server.getType(), nexilis::Protocol::Type::BOOST_TCP_SERVER);
    EXPECT_EQ(boost_tcp_server.getSettings().getMode(), nexilis::server::AuthenticationMode::password_protected);
    EXPECT_EQ(boost_tcp_server.getSettings().getPassphrase(), "salasana");
    EXPECT_EQ(boost_tcp_server.getSettings().getRootPassword(), "root");
}
