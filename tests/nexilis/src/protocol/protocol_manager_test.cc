#include <gtest/gtest.h>

#include <nexilis/protocol.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server/protocol/af_unix/stream_server.hh>
#include <nexilis/server/settings.hh>

class ProtocolManagerTest : public ::testing::Test
{
protected:
    nexilis::ProtocolManager manager;
};

TEST_F(ProtocolManagerTest, CreateProtocol_UnixStreamServer)
{
    nexilis::server::Settings settings;
    settings.setMode(nexilis::server::Settings::AuthenticationMode::passwordProtected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    auto unix_stream_server = manager.createProtocol<nexilis::server::af_unix::StreamServer>(settings, "/tmp/nexilis/stream");

    EXPECT_EQ(unix_stream_server.getType(), nexilis::Protocol::Type::AF_UNIX_SOCK_STREAM_SERVER);
    EXPECT_EQ(unix_stream_server.getSettings().getMode(), nexilis::server::Settings::AuthenticationMode::passwordProtected);
    EXPECT_EQ(unix_stream_server.getSettings().getPassphrase(), "salasana");
    EXPECT_EQ(unix_stream_server.getSettings().getRootPassword(), "root");
}
