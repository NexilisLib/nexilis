using System;
using Xunit;
using Nexilis;
using Nexilis.Client;

namespace Nexilis.Tests
{
    public class RoomDataTests : IDisposable
    {
        private readonly RoomData _roomData;

        public RoomDataTests()
        {
            _roomData = new RoomData(100, "TestRoom", RoomContext.ROOM_CONTEXT_2D, 10);
        }

        public void Dispose()
        {
            _roomData.Dispose();
        }

        [Fact]
        public void CreateWithAllParams()
        {
            using var room = new RoomData(100, "TestRoom", RoomContext.ROOM_CONTEXT_2D, 10);

            Assert.Equal(100ul, room.CreatorId);
            Assert.Equal("TestRoom", room.Name);
            Assert.Equal(RoomContext.ROOM_CONTEXT_2D, room.Context);
            Assert.Equal(10u, room.MaxSize);
            Assert.NotEqual(0ul, room.Id);
        }

        [Fact]
        public void CreateWith3DContext()
        {
            using var room = new RoomData(1, "MyRoom", RoomContext.ROOM_CONTEXT_3D, 20);

            Assert.Equal(1ul, room.CreatorId);
            Assert.Equal("MyRoom", room.Name);
            Assert.Equal(RoomContext.ROOM_CONTEXT_3D, room.Context);
            Assert.Equal(20u, room.MaxSize);
        }

        [Fact]
        public void CreateWithEmptyNameThrows()
        {
            Assert.Throws<ArgumentException>(() => new RoomData(0, "", RoomContext.ROOM_CONTEXT_2D, 10));
        }

        [Fact]
        public void UniqueIds()
        {
            using var a = new RoomData(0, "RoomA", RoomContext.ROOM_CONTEXT_2D, 10);
            using var b = new RoomData(0, "RoomB", RoomContext.ROOM_CONTEXT_3D, 5);

            Assert.NotEqual(a.Id, b.Id);
        }

        [Fact]
        public void ContextEnumValues()
        {
            using var room2D = new RoomData(0, "R2D", RoomContext.ROOM_CONTEXT_2D, 10);
            using var room3D = new RoomData(0, "R3D", RoomContext.ROOM_CONTEXT_3D, 10);

            Assert.Equal(RoomContext.ROOM_CONTEXT_2D, room2D.Context);
            Assert.Equal(RoomContext.ROOM_CONTEXT_3D, room3D.Context);
        }

        [Fact]
        public void AccessAfterDisposeThrows()
        {
            _roomData.Dispose();
            Assert.Throws<ObjectDisposedException>(() => _roomData.Name);
        }
    }
}
