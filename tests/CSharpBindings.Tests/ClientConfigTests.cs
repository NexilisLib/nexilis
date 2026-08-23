using Xunit;
using Nexilis.Client;

namespace Nexilis.Tests
{
    public class ClientConfigTests
    {
        [Fact]
        public void CreateAndDispose()
        {
            using var config = new ClientConfig();
            Assert.NotEqual(System.IntPtr.Zero, config.ConfigPtr);
        }

        [Fact]
        public void PasswordSetGet()
        {
            using var config = new ClientConfig();
            config.SetPassword("my_password");

            Assert.Equal("my_password", config.GetPassword());
        }

        [Fact]
        public void Mode()
        {
            using var config = new ClientConfig();
            // 2 = password_protected.
            config.SetMode(2);
        }

        [Fact]
        public void BoostTCPAddress()
        {
            using var config = new ClientConfig();
            config.SetBoostTCPAddress("192.168.1.100");

            Assert.Equal("192.168.1.100", config.GetBoostTCPServerAddress());
        }

        [Fact]
        public void BoostUDPAddress()
        {
            using var config = new ClientConfig();
            config.SetBoostUDP("10.0.0.1");

            Assert.Equal("10.0.0.1", config.GetBoostUdpServerAddress());
        }

        [Fact]
        public void FluentSettersReturnSameInstance()
        {
            using var config = new ClientConfig();

            var returned = config.Password("pw").BoostTCP("1.2.3.4").BoostUDP("5.6.7.8");

            Assert.Same(config, returned);
            Assert.Equal("pw", config.GetPassword());
            Assert.Equal("1.2.3.4", config.GetBoostTCPServerAddress());
            Assert.Equal("5.6.7.8", config.GetBoostUdpServerAddress());
        }
    }
}
