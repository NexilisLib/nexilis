using System;
using Xunit;
using Nexilis;

public class BoostTCPServerTest
{
    [Fact]
    public void CreateProtocol_BoostTCPServer()
    {
        // Arrange
        using (var settings = new Settings())
        using (var protocolManager = new ProtocolManager())
        {
            // Set up settings
            settings.SetMode(AuthenticationMode.PasswordProtected);
            settings.SetPassphrase("salasana");
            settings.SetRootPassword("root");

            int port = 12345;

            using (var server = new BoostTCPServer(protocolManager, settings, port))
            {
                // Assert
                // Verify the protocol type
                Assert.Equal(ProtocolType.BOOST_TCP_SERVER, server.GetProtocolType());

                // Verify the settings
                var serverSettings = server.GetSettings();
                Assert.Equal(AuthenticationMode.PasswordProtected, serverSettings.GetMode());
                Assert.Equal("salasana", serverSettings.GetPassphrase());
                Assert.Equal("root", serverSettings.GetRootPassword());
            }
        }
    }
}