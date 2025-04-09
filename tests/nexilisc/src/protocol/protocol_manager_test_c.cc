#include <gtest/gtest.h>

#include <nexilisc/protocol_manager_c.h>

#include <nexilisc/server/protocol/boost_tcp_server_c.h>
#include <nexilisc/server/protocol/unix_stream_server_c.h>

TEST(ProtocolTest_c, CreateProtocol_UnixStreamServer)
{
    nexilis_server_SettingsC* settings = nexilis_settings_create();
    nexilis_settings_set_mode(settings, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
    nexilis_settings_set_passphrase(settings, "salasana");
    nexilis_settings_set_root_password(settings, "root");

    auto protocol_manager = nexilis_protocol_manager_create();

    auto server = nexilis_create_unix_stream_server(protocol_manager, settings, "/tmp/nexilis/stream");
    auto protocol_settings = nexilis_unix_stream_server_get_settings(server);

    EXPECT_EQ(nexilis_unix_stream_server_get_type(server), PROTOCOL_TYPE_AF_UNIX_SOCK_STREAM_SERVER);
    EXPECT_EQ(protocol_settings.settings->getMode(), nexilis::server::Settings::AuthenticationMode::passwordProtected);
    EXPECT_EQ(protocol_settings.settings->getPassphrase(), "salasana");
    EXPECT_EQ(protocol_settings.settings->getRootPassword(), "root");

    nexilis_settings_destroy(settings);
    nexilis_protocol_manager_destroy(protocol_manager);
    nexilis_unix_stream_server_destroy(server);
}

TEST(ProtocolTest_c, CreateProtocol_BoostTCPServer)
{
    nexilis_server_SettingsC* settings = nexilis_settings_create();
    nexilis_settings_set_mode(settings, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
    nexilis_settings_set_passphrase(settings, "salasana");
    nexilis_settings_set_root_password(settings, "root");

    auto protocol_manager = nexilis_protocol_manager_create();

    auto server = nexilis_create_boost_tcp_server(protocol_manager, settings);
    auto protocol_settings = nexilis_boost_tcp_server_get_settings(server);

    EXPECT_EQ(nexilis_boost_tcp_server_get_type(server), PROTOCOL_TYPE_BOOST_TCP_SERVER);
    EXPECT_EQ(protocol_settings.settings->getMode(), nexilis::server::Settings::AuthenticationMode::passwordProtected);
    EXPECT_EQ(protocol_settings.settings->getPassphrase(), "salasana");
    EXPECT_EQ(protocol_settings.settings->getRootPassword(), "root");

    nexilis_settings_destroy(settings);
    nexilis_protocol_manager_destroy(protocol_manager);
    nexilis_boost_tcp_server_destroy(server);
}
