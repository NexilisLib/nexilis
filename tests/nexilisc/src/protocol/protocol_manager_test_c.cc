#include <gtest/gtest.h>

#include <nexilis/protocol_manager.hh>
#include <nexilisc/protocol_manager_c.h>

TEST(ProtocolTest_c, CreateProtocol_UnixStreamServer)
{
    nexilis_server_SettingsC* settings = nexilis_settings_create();
    nexilis_settings_set_mode(settings, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
    nexilis_settings_set_passphrase(settings, "salasana");
    nexilis_settings_set_root_password(settings, "root");

    auto protocol_manager = nexilis_protocol_manager_create();

    auto server = nexilis_protocol_create_unix_stream_server(protocol_manager, settings, "/tmp/nexilis/stream");

    nexilis_settings_destroy(settings);
    nexilis_protocol_manager_destroy(protocol_manager);
    nexilis_protocol_unix_stream_server_destroy(server);
}
