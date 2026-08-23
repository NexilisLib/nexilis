using System;
using Xunit;
using Nexilis.Client;

namespace Nexilis.Tests
{
    public class ClientSessionTests : IDisposable
    {
        private readonly ClientConfig _config;
        private readonly ClientAPI _clientApi;

        public ClientSessionTests()
        {
            TestEnvironment.Initialize();
            _config = new ClientConfig();
            _clientApi = new ClientAPI(_config);
        }

        public void Dispose()
        {
            _clientApi.Dispose();
            _config.Dispose();
        }

        [Fact]
        public void CreateAndDestroy()
        {
            using var session = new ClientSession(42, _clientApi.ClientApiPtr, true);
        }

        [Fact]
        public void GetId()
        {
            using var session = new ClientSession(42, _clientApi.ClientApiPtr, true);

            Assert.Equal(42ul, session.GetId());
        }

        [Fact]
        public void GetIdDifferentValues()
        {
            using var s1 = new ClientSession(1, _clientApi.ClientApiPtr, true);
            using var s2 = new ClientSession(999, _clientApi.ClientApiPtr, true);

            Assert.Equal(1ul, s1.GetId());
            Assert.Equal(999ul, s2.GetId());
        }

        [Fact]
        public void SetAndGetPosition3D()
        {
            using var session = new ClientSession(1, _clientApi.ClientApiPtr, true);
            session.SetPosition3D(1.0f, 2.0f, 3.0f);

            using var pos = session.GetPosition3D();
            Assert.Equal(1.0f, pos.X);
            Assert.Equal(2.0f, pos.Y);
            Assert.Equal(3.0f, pos.Z);
        }

        [Fact]
        public void SetUsername()
        {
            using var session = new ClientSession(1, _clientApi.ClientApiPtr, true);
            // No getter for username via the API, but this should not throw.
            session.SetUsername("test_player");
        }

        [Fact]
        public void NullClientApiHandleThrows()
        {
            Assert.Throws<ArgumentNullException>(() => new ClientSession(1, IntPtr.Zero, true));
        }
    }
}
