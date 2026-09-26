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
using System.Threading.Tasks;
using Nexilis.Client;
using Nexilis.Util;
using Xunit;

namespace Nexilis.Tests
{
    /// <summary>
    /// Covers the NexilisClient surface that does not need a live server:
    /// defaults, lifecycle teardown, argument validation and the guards that
    /// keep the API usable before a connection exists. The connection attempt
    /// itself is covered by NexilisConnectionTests.
    /// </summary>
    public class NexilisClientTests : IDisposable
    {
        readonly NexilisClient _client = new NexilisClient();

        public NexilisClientTests()
        {
            TestEnvironment.Initialize();
        }

        public void Dispose()
        {
            _client.Dispose();
        }

        /// <summary>A client that is already torn down.</summary>
        static NexilisClient DisposedClient()
        {
            var client = new NexilisClient();
            client.Dispose();
            return client;
        }

        // --- Defaults ---

        [Fact]
        public void Defaults()
        {
            Assert.Equal("127.0.0.1", _client.ServerAddress);
            Assert.Equal("password", _client.Password);
            Assert.Equal(2, _client.AuthenticationMode);
            Assert.True(_client.AutoJoinOnConnect);
            Assert.Null(_client.PreferredRoomId);
            Assert.Equal(5000, _client.RoomJoinTimeoutMs);
            Assert.Equal(50, _client.RoomPollIntervalMs);
        }

        [Fact]
        public void StartsUnconnected()
        {
            Assert.False(_client.IsDisposed);
            Assert.False(_client.IsReady);
            Assert.Equal(0ul, _client.ClientId);
            Assert.Equal(0ul, _client.ClientRoomId);
            Assert.Null(_client.ClientRoom);
            Assert.Null(_client.ClientRoomInfo);
            Assert.Null(_client.ClientApi);
            Assert.Null(_client.TcpClient);
            Assert.Null(_client.UdpClient);
            Assert.Null(_client.Logger);
        }

        [Fact]
        public void NoRemoteClientsBeforeConnect()
        {
            Assert.Equal(0, _client.RemoteClientCount);
            Assert.Empty(_client.GetRemoteClientIds());
        }

        [Fact]
        public void UpdateBeforeConnectIsANoOp()
        {
            // Must not throw even though nothing is connected yet.
            _client.Update();
        }

        // --- Room listing needs a connection ---

        [Fact]
        public void GetAvailableRooms_Throws_WhenNotConnected()
        {
            Assert.Throws<InvalidOperationException>(() => _client.GetAvailableRooms());
        }

        [Fact]
        public async Task GetAvailableRoomsAsync_Throws_WhenNotConnected()
        {
            await Assert.ThrowsAsync<InvalidOperationException>(() => _client.GetAvailableRoomsAsync());
        }

        [Fact]
        public void GetAvailableRoomCount_Throws_WhenNotConnected()
        {
            Assert.Throws<InvalidOperationException>(() => _client.GetAvailableRoomCount());
        }

        [Fact]
        public void TryGetRoomInfo_Throws_WhenNotConnected()
        {
            Assert.Throws<InvalidOperationException>(() => _client.TryGetRoomInfo(1, out _));
        }

        [Fact]
        public async Task JoinAnyRoomAsync_Throws_WhenNotConnected()
        {
            await Assert.ThrowsAsync<InvalidOperationException>(() => _client.JoinAnyRoomAsync());
        }

        [Fact]
        public async Task LeaveRoomAsync_Throws_WhenNotConnected()
        {
            await Assert.ThrowsAsync<InvalidOperationException>(() => _client.LeaveRoomAsync());
        }

        [Fact]
        public async Task JoinRoomAsync_Throws_WhenNotConnected()
        {
            await Assert.ThrowsAsync<InvalidOperationException>(() => _client.JoinRoomAsync(42));
        }

        [Fact]
        public async Task CreateRoomAsync_Throws_WhenNotConnected()
        {
            await Assert.ThrowsAsync<InvalidOperationException>(() => _client.CreateRoomAsync("TestRoom"));
        }

        [Fact]
        public void GetRemoteClient_Throws_WhenNotConnected()
        {
            Assert.Throws<InvalidOperationException>(() => _client.GetRemoteClient(1));
        }

        [Fact]
        public void GetClientUsername_Throws_WhenNotConnected()
        {
            Assert.Throws<InvalidOperationException>(() => _client.GetClientUsername(1));
        }

        // --- Sending whole packages ---

        [Fact]
        public void SendPacket_NullData_Throws()
        {
            Assert.Throws<ArgumentNullException>(() => _client.SendPacket((NxData)null!, PacketTransport.Tcp));
            Assert.Throws<ArgumentNullException>(() => _client.SendPacket((NxData)null!, PacketTransport.Udp));
            Assert.Throws<ArgumentNullException>(() => _client.SendPacket((byte[])null!, PacketTransport.Tcp));
            Assert.Throws<ArgumentNullException>(() => _client.SendPacket((byte[])null!, PacketTransport.Udp));
        }

        [Fact]
        public void SendPacket_EmptyData_Throws()
        {
            // Rejected on the argument check, before the connection is even
            // looked at, so this works without a server.
            Assert.Throws<ArgumentException>(() => _client.SendPacket(Array.Empty<byte>(), PacketTransport.Tcp));
            Assert.Throws<ArgumentException>(() => _client.SendPacket(Array.Empty<byte>(), PacketTransport.Udp));
        }

        [Fact]
        public void SendPacket_Throws_WhenNotConnected()
        {
            var data = new byte[] { 1, 2, 3 };

            Assert.Throws<InvalidOperationException>(() => _client.SendPacket(data, PacketTransport.Tcp));
            Assert.Throws<InvalidOperationException>(() => _client.SendPacket(data, PacketTransport.Udp));
            Assert.Throws<InvalidOperationException>(() => _client.SendPacketTcp(data));
            Assert.Throws<InvalidOperationException>(() => _client.SendPacketUdp(data));
        }

        [Fact]
        public void SendPacketTcp_NullData_Throws()
        {
            Assert.Throws<ArgumentNullException>(() => _client.SendPacketTcp((NxData)null!));
            Assert.Throws<ArgumentNullException>(() => _client.SendPacketTcp((byte[])null!));
        }

        [Fact]
        public void SendPacketUdp_NullData_Throws()
        {
            Assert.Throws<ArgumentNullException>(() => _client.SendPacketUdp((NxData)null!));
            Assert.Throws<ArgumentNullException>(() => _client.SendPacketUdp((byte[])null!));
        }

        [Fact]
        public void SendPosition_Throws_WhenNotReady()
        {
            Assert.Throws<InvalidOperationException>(() => _client.SendPosition(1f, 2f, 3f));
            Assert.Throws<InvalidOperationException>(() => _client.SendPosition(new Position(1f, 2f, 3f)));
        }

        [Fact]
        public void SendMovement_Throws_WhenNotReady()
        {
            Assert.Throws<InvalidOperationException>(() => _client.SendMovement(1f, 2f, 3f, 0.016f));
            Assert.Throws<InvalidOperationException>(() => _client.SendMovement(new Position(1f, 2f, 3f), 0.016f));
        }

        // --- Argument validation happens before the state check ---

        [Fact]
        public async Task JoinRoomAsync_ZeroRoomId_Throws()
        {
            await Assert.ThrowsAsync<ArgumentException>(() => _client.JoinRoomAsync(0));
        }

        [Fact]
        public async Task CreateRoomAsync_EmptyName_Throws()
        {
            await Assert.ThrowsAsync<ArgumentException>(() => _client.CreateRoomAsync(""));
            await Assert.ThrowsAsync<ArgumentException>(() => _client.CreateRoomAsync("   "));
            await Assert.ThrowsAsync<ArgumentException>(() => _client.CreateRoomAsync(null!));
        }

        // --- Teardown ---

        [Fact]
        public void Disconnect_RaisesOnDisconnected()
        {
            int calls = 0;
            var client = new NexilisClient { OnDisconnected = () => calls++ };

            client.Disconnect();

            Assert.Equal(1, calls);
            Assert.True(client.IsDisposed);
        }

        [Fact]
        public void Disconnect_IsIdempotent()
        {
            int calls = 0;
            var client = new NexilisClient { OnDisconnected = () => calls++ };

            client.Disconnect();
            client.Disconnect();
            client.Dispose();

            Assert.Equal(1, calls);
        }

        [Fact]
        public void Dispose_ClearsState()
        {
            var client = new NexilisClient();

            client.Dispose();

            Assert.True(client.IsDisposed);
            Assert.False(client.IsReady);
            Assert.Equal(0ul, client.ClientId);
            Assert.Equal(0ul, client.ClientRoomId);
            Assert.Null(client.ClientRoom);
            Assert.Null(client.ClientRoomInfo);
            Assert.Null(client.ClientApi);
            Assert.Equal(0, client.RemoteClientCount);
        }

        [Fact]
        public async Task DisposedClient_RejectsConnect()
        {
            var client = DisposedClient();
            await Assert.ThrowsAsync<ObjectDisposedException>(() => client.ConnectAsync());
        }

        [Fact]
        public void DisposedClient_RejectsRoomQueries()
        {
            var client = DisposedClient();

            Assert.Throws<ObjectDisposedException>(() => client.GetAvailableRooms());
            Assert.Throws<ObjectDisposedException>(() => client.GetAvailableRoomCount());
            Assert.Throws<ObjectDisposedException>(() => client.TryGetRoomInfo(1, out _));
            Assert.Throws<ObjectDisposedException>(() => client.GetRemoteClient(1));
            Assert.Throws<ObjectDisposedException>(() => client.GetClientUsername(1));
        }

        [Fact]
        public async Task DisposedClient_RejectsRoomCommands()
        {
            var client = DisposedClient();

            await Assert.ThrowsAsync<ObjectDisposedException>(() => client.JoinAnyRoomAsync());
            await Assert.ThrowsAsync<ObjectDisposedException>(() => client.JoinRoomAsync(1));
            await Assert.ThrowsAsync<ObjectDisposedException>(() => client.LeaveRoomAsync());
            await Assert.ThrowsAsync<ObjectDisposedException>(() => client.CreateRoomAsync("Room"));
            await Assert.ThrowsAsync<ObjectDisposedException>(() => client.GetAvailableRoomsAsync());
        }

        [Fact]
        public void DisposedClient_RejectsSending()
        {
            var client = DisposedClient();
            var data = new byte[] { 1, 2, 3 };

            Assert.Throws<ObjectDisposedException>(() => client.SendPacket(data, PacketTransport.Tcp));
            Assert.Throws<ObjectDisposedException>(() => client.SendPacket(data, PacketTransport.Udp));
            Assert.Throws<ObjectDisposedException>(() => client.SendPosition(1f, 2f, 3f));
            Assert.Throws<ObjectDisposedException>(() => client.SendMovement(1f, 2f, 3f, 0.016f));
        }

        [Fact]
        public void DisposedClient_UpdateIsANoOp()
        {
            DisposedClient().Update();
        }
    }
}
