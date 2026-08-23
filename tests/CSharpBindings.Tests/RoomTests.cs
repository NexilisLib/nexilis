using Xunit;
using Nexilis;
using Nexilis.Client;

namespace Nexilis.Tests
{
    public class RoomTests : IDisposable
    {
        private readonly ClientConfig _config;
        private readonly ClientAPI _clientApi;
        private readonly RoomData _roomData;
        private readonly Room _room;

        public RoomTests()
        {
            TestEnvironment.Initialize();
            _config = new ClientConfig();
            _clientApi = new ClientAPI(_config);
            _roomData = new RoomData(100, "TestRoom", RoomContext.ROOM_CONTEXT_3D, 10);
            _room = new Room(_roomData);
        }

        public void Dispose()
        {
            _room.Dispose();
            _roomData.Dispose();
            _clientApi.Dispose();
            _config.Dispose();
        }

        // The room takes ownership of added sessions (moved in natively),
        // so they must not be disposed by the caller afterwards.
        private ClientSession MakeSession(ulong id)
        {
            return new ClientSession(id, _clientApi.ClientApiPtr, false);
        }

        [Fact]
        public void CreateAndDestroy()
        {
            using var room = new Room(_roomData);
            Assert.Equal(0ul, room.GetClientAmount());
        }

        [Fact]
        public void GetId()
        {
            Assert.Equal(_roomData.Id, _room.GetId());
        }

        [Fact]
        public void AddClient()
        {
            var client = MakeSession(10);
            _room.AddClient(client);

            Assert.Equal(1ul, _room.GetClientAmount());
        }

        [Fact]
        public void AddMultipleClients()
        {
            _room.AddClient(MakeSession(10));
            _room.AddClient(MakeSession(20));

            Assert.Equal(2ul, _room.GetClientAmount());
        }

        [Fact]
        public void RemoveClient()
        {
            _room.AddClient(MakeSession(10));
            _room.AddClient(MakeSession(20));

            _room.RemoveClient(10);

            Assert.Equal(1ul, _room.GetClientAmount());
        }

        [Fact]
        public void GetClients()
        {
            _room.AddClient(MakeSession(10));

            var clients = _room.GetClients();

            Assert.Single(clients);
            Assert.Equal(10ul, clients[0].GetId());
        }

        [Fact]
        public void GetClientsEmpty()
        {
            var clients = _room.GetClients();

            Assert.Empty(clients);
        }

        [Fact]
        public void AddNullClientThrows()
        {
            Assert.Throws<System.ArgumentNullException>(() => _room.AddClient(null));
        }
    }
}
