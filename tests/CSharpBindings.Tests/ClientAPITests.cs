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
    /// Covers ClientAPI and RoomsCollection. A freshly created ClientAPI has
    /// talked to no server, so it knows no rooms and no clients, which is
    /// exactly the state the room listing helpers have to handle.
    /// </summary>
    public class ClientAPITests : IDisposable
    {
        private readonly ClientConfig _config;
        private readonly ClientAPI _clientApi;

        public ClientAPITests()
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

        // --- ClientAPI ---

        [Fact]
        public void CreateRequiresAConfig()
        {
            Assert.Throws<ArgumentNullException>(() => new ClientAPI(null!));
        }

        [Fact]
        public void NotInitializedBeforeConnecting()
        {
            Assert.False(_clientApi.IsInitialized());
        }

        [Fact]
        public void NoRoomsBeforeConnecting()
        {
            Assert.Equal(0ul, _clientApi.GetActiveRoomsCount());
            Assert.Empty(_clientApi.GetActiveRoomInfos());
        }

        [Fact]
        public void GetActiveRoomsIsEmptyNotNull()
        {
            using var rooms = _clientApi.GetActiveRooms();

            Assert.Equal(0ul, rooms.Count);
            Assert.True(rooms.IsEmpty);
            Assert.Empty(rooms.GetRooms());
            Assert.Empty(rooms.GetRoomInfos());
        }

        [Fact]
        public void GetRoomFromUnknownIdReturnsNull()
        {
            Assert.Null(_clientApi.GetRoomFromId(1234));
        }

        [Fact]
        public void GetClientFromUnknownIdThrows()
        {
            Assert.Throws<InvalidOperationException>(() => _clientApi.GetClientFromClientId(1234));
        }

        [Fact]
        public void GetClientUsernameOfUnknownClientIsEmpty()
        {
            Assert.Equal(string.Empty, _clientApi.GetClientUsername(1234));
        }

        [Fact]
        public void NotInRoomBeforeJoining()
        {
            Assert.False(_clientApi.IsInRoom());
            Assert.Equal(0ul, _clientApi.ClientRoomId());
        }

        [Fact]
        public void DisposeIsIdempotent()
        {
            _clientApi.Dispose();
            _clientApi.Dispose();

            Assert.True(_clientApi.IsDisposed);
        }

        [Fact]
        public void AccessAfterDisposeThrows()
        {
            _clientApi.Dispose();

            Assert.Throws<ObjectDisposedException>(() => _clientApi.IsInitialized());
            Assert.Throws<ObjectDisposedException>(() => _clientApi.GetActiveRooms());
            Assert.Throws<ObjectDisposedException>(() => _clientApi.GetActiveRoomInfos());
            Assert.Throws<ObjectDisposedException>(() => _clientApi.GetActiveRoomsCount());
            Assert.Throws<ObjectDisposedException>(() => _clientApi.GetClientId());
            Assert.Throws<ObjectDisposedException>(() => _clientApi.GetClientUsername(1));
            Assert.Throws<ObjectDisposedException>(() => _clientApi.ClientRoomId());
            Assert.Throws<ObjectDisposedException>(() => _clientApi.GetRoomFromId(1));
            Assert.Throws<ObjectDisposedException>(() => _clientApi.GetClientFromClientId(1));
            Assert.Throws<ObjectDisposedException>(() => _clientApi.ClientApiPtr);
        }

        // --- RoomsCollection ---

        [Fact]
        public void RoomsCollectionRejectsANullHandle()
        {
            Assert.Throws<ArgumentNullException>(() => new RoomsCollection(IntPtr.Zero));
        }

        [Fact]
        public void RoomsCollectionGetRoomOutOfRangeThrows()
        {
            using var rooms = _clientApi.GetActiveRooms();

            Assert.Throws<ArgumentOutOfRangeException>(() => rooms.GetRoom(0));
        }

        [Fact]
        public void RoomsCollectionAccessAfterDisposeThrows()
        {
            var rooms = _clientApi.GetActiveRooms();
            rooms.Dispose();

            Assert.Throws<ObjectDisposedException>(() => rooms.GetRooms());
            Assert.Throws<ObjectDisposedException>(() => rooms.GetRoomInfos());
            Assert.Throws<ObjectDisposedException>(() => rooms.GetRoom(0));
        }

        [Fact]
        public void RoomsCollectionDisposeIsIdempotent()
        {
            var rooms = _clientApi.GetActiveRooms();

            rooms.Dispose();
            rooms.Dispose();
        }
    }
}
