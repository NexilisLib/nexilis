/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

using System;
using Nexilis.Client;
using Xunit;

namespace Nexilis.Tests
{
    /// <summary>
    /// Covers the room metadata snapshot used by NexilisClient to list the
    /// rooms a server exposes.
    /// </summary>
    public class RoomInfoTests : IDisposable
    {
        private readonly ClientConfig _config;
        private readonly ClientAPI _clientApi;

        public RoomInfoTests()
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
        public void ConstructorMapsEveryField()
        {
            var info = new RoomInfo(7, "Lobby", RoomContext.ROOM_CONTEXT_2D, 12, 5, 99);

            Assert.Equal(7ul, info.Id);
            Assert.Equal("Lobby", info.Name);
            Assert.Equal(RoomContext.ROOM_CONTEXT_2D, info.Context);
            Assert.Equal(12u, info.MaxSize);
            Assert.Equal(5ul, info.ClientCount);
            Assert.Equal(99ul, info.CreatorId);
        }

        [Fact]
        public void CreatorIdDefaultsToZero()
        {
            var info = new RoomInfo(1, "R", RoomContext.ROOM_CONTEXT_3D, 4, 0);

            Assert.Equal(0ul, info.CreatorId);
        }

        [Fact]
        public void NullNameBecomesEmpty()
        {
            var info = new RoomInfo(1, null!, RoomContext.ROOM_CONTEXT_3D, 4, 0);

            Assert.Equal(string.Empty, info.Name);
        }

        [Fact]
        public void IsFullWhenClientCountReachesMaxSize()
        {
            Assert.True(new RoomInfo(1, "Full", RoomContext.ROOM_CONTEXT_3D, 2, 2).IsFull);
            Assert.True(new RoomInfo(1, "Over", RoomContext.ROOM_CONTEXT_3D, 2, 3).IsFull);
            Assert.False(new RoomInfo(1, "Free", RoomContext.ROOM_CONTEXT_3D, 2, 1).IsFull);
        }

        [Fact]
        public void HasFreeSpaceIsTheInverseOfIsFull()
        {
            var full = new RoomInfo(1, "Full", RoomContext.ROOM_CONTEXT_3D, 2, 2);
            var free = new RoomInfo(1, "Free", RoomContext.ROOM_CONTEXT_3D, 2, 1);

            Assert.False(full.HasFreeSpace);
            Assert.True(free.HasFreeSpace);
        }

        [Fact]
        public void UnknownMaxSizeNeverCountsAsFull()
        {
            // MaxSize 0 means "the server did not tell us", so the room must
            // not be reported as full.
            var info = new RoomInfo(1, "Unknown", RoomContext.ROOM_CONTEXT_3D, 0, 100);

            Assert.False(info.IsFull);
            Assert.True(info.HasFreeSpace);
        }

        [Fact]
        public void IsEmptyReflectsTheClientCount()
        {
            Assert.True(new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_3D, 4, 0).IsEmpty);
            Assert.False(new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_3D, 4, 1).IsEmpty);
        }

        [Fact]
        public void FromRoomReadsEveryValue()
        {
            using var roomData = new RoomData(42, "Arena", RoomContext.ROOM_CONTEXT_3D, 8);
            using var room = new Room(roomData);

            var info = RoomInfo.FromRoom(room);

            Assert.Equal(roomData.Id, info.Id);
            Assert.Equal("Arena", info.Name);
            Assert.Equal(RoomContext.ROOM_CONTEXT_3D, info.Context);
            Assert.Equal(8u, info.MaxSize);
            Assert.Equal(42ul, info.CreatorId);
            Assert.Equal(0ul, info.ClientCount);
        }

        [Fact]
        public void FromRoomCountsClients()
        {
            using var roomData = new RoomData(1, "Busy", RoomContext.ROOM_CONTEXT_2D, 4);
            using var room = new Room(roomData);
            room.AddClient(new ClientSession(10, _clientApi.ClientApiPtr, false));
            room.AddClient(new ClientSession(20, _clientApi.ClientApiPtr, false));

            var info = RoomInfo.FromRoom(room);

            Assert.Equal(2ul, info.ClientCount);
            Assert.True(info.HasFreeSpace);
        }

        [Fact]
        public void FromRoomSurvivesTheRoomBeingDisposed()
        {
            using var roomData = new RoomData(1, "Temp", RoomContext.ROOM_CONTEXT_3D, 4);
            var room = new Room(roomData);
            var info = RoomInfo.FromRoom(room);

            room.Dispose();

            // The snapshot is plain managed data, so it stays readable.
            Assert.Equal("Temp", info.Name);
            Assert.Equal(4u, info.MaxSize);
        }

        [Fact]
        public void FromRoomNullThrows()
        {
            Assert.Throws<ArgumentNullException>(() => RoomInfo.FromRoom(null!));
        }

        [Fact]
        public void FromRoomDisposedRoomThrows()
        {
            using var roomData = new RoomData(1, "Gone", RoomContext.ROOM_CONTEXT_3D, 4);
            var room = new Room(roomData);
            room.Dispose();

            Assert.Throws<ObjectDisposedException>(() => RoomInfo.FromRoom(room));
        }

        [Fact]
        public void EqualityComparesEveryField()
        {
            var a = new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_2D, 4, 1, 5);
            var b = new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_2D, 4, 1, 5);

            Assert.Equal(a, b);
            Assert.True(a.Equals(b));
            Assert.True(a.Equals((object)b));
            Assert.Equal(a.GetHashCode(), b.GetHashCode());
        }

        [Fact]
        public void DifferentValuesAreNotEqual()
        {
            var baseInfo = new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_2D, 4, 1, 5);

            Assert.NotEqual(baseInfo, new RoomInfo(2, "A", RoomContext.ROOM_CONTEXT_2D, 4, 1, 5));
            Assert.NotEqual(baseInfo, new RoomInfo(1, "B", RoomContext.ROOM_CONTEXT_2D, 4, 1, 5));
            Assert.NotEqual(baseInfo, new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_3D, 4, 1, 5));
            Assert.NotEqual(baseInfo, new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_2D, 5, 1, 5));
            Assert.NotEqual(baseInfo, new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_2D, 4, 2, 5));
            Assert.NotEqual(baseInfo, new RoomInfo(1, "A", RoomContext.ROOM_CONTEXT_2D, 4, 1, 6));
            Assert.False(baseInfo.Equals(null));
        }

        [Fact]
        public void ToStringMentionsTheName()
        {
            var info = new RoomInfo(1, "Lobby", RoomContext.ROOM_CONTEXT_3D, 4, 2);

            Assert.Contains("Lobby", info.ToString());
        }
    }
}
