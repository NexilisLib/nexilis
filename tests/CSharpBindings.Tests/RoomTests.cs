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

        // --- Communication ---

        [Fact]
        public void CommunicationCreateAndPayload()
        {
            var sender = MakeSession(10);
            using var comm = new Communication("hello world", sender);

            // Ids are generated at construction time.
            Assert.NotEqual(0ul, comm.GetId());
            Assert.Equal("hello world", comm.GetPayload());
        }

        [Fact]
        public void CommunicationCreateNullSenderThrows()
        {
            Assert.Throws<ArgumentNullException>(() => new Communication("test", null));
        }

        [Fact]
        public void CommunicationCreateNullPayloadThrows()
        {
            var sender = MakeSession(10);
            Assert.Throws<ArgumentException>(() => new Communication(null, sender));
        }

        [Fact]
        public void CommunicationAddMessage()
        {
            var sender = MakeSession(10);
            var comm = new Communication("test message", sender);

            _room.AddMessage(comm);

            Assert.NotEqual(0ul, comm.GetId());
            Assert.True(_room.ContainsCommunication(comm.GetId()));
        }

        [Fact]
        public void ContainsCommunication()
        {
            var sender = MakeSession(10);
            using var comm = new Communication("msg", sender);

            Assert.False(_room.ContainsCommunication(comm.GetId()));

            var added = new Communication("added message", sender);
            _room.AddMessage(added);
            // Check by id: object equality is unreliable after the
            // native move into the room.
            Assert.True(_room.ContainsCommunication(added.GetId()));
        }

        [Fact]
        public void GetMessages()
        {
            var sender = MakeSession(10);
            _room.AddMessage(new Communication("first", sender));
            _room.AddMessage(new Communication("second", sender));

            var messages = _room.GetMessages();

            Assert.Equal(2, messages.Count);

            // GetMessages returns copies that the caller owns.
            var payloads = new List<string>();
            foreach (var message in messages)
            {
                using (message)
                {
                    payloads.Add(message.GetPayload());
                }
            }
            payloads.Sort();
            Assert.Equal(new List<string> { "first", "second" }, payloads);
        }

        [Fact]
        public void GetMessagesEmpty()
        {
            Assert.Empty(_room.GetMessages());
        }

        [Fact]
        public void CommunicationDisposeIsIdempotent()
        {
            var sender = MakeSession(10);
            var comm = new Communication("test", sender);

            comm.Dispose();
            comm.Dispose();

            Assert.Throws<ObjectDisposedException>(() => comm.GetPayload());
        }
    }
}
