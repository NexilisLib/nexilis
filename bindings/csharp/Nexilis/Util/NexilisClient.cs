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
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Nexilis.Client;

namespace Nexilis.Util
{
    /// <summary>
    /// High level, engine-agnostic client for the Nexilis bindings. Wraps the
    /// full connection lifecycle (config, BoostTCP / BoostUDP clients, room
    /// join) and exposes the connected room as plain data through callbacks,
    /// so it can be driven from any game loop.
    ///
    /// Wire the callbacks from your engine layer with anonymous functions,
    /// e.g. in Unity:
    /// <code>
    /// client = new NexilisClient
    /// {
    ///     ServerAddress = "127.0.0.1",
    ///     LogHandler = (level, msg) => Debug.Log($"[Nexilis] {msg}"),
    /// };
    /// client.OnReady += () => Debug.Log("Connected!");
    /// client.OnRemoteClientPosition += (id, pos) => remoteGo.transform.position = new Vector3(pos.X, pos.Y, pos.Z);
    /// await client.ConnectAsync();
    /// </code>
    /// There is no Unity code in this class on purpose.
    /// </summary>
    public class NexilisClient : IDisposable
    {
        /// <summary>Server address, e.g. "127.0.0.1". Set before <see cref="ConnectAsync"/>.</summary>
        public string ServerAddress { get; set; } = "127.0.0.1";

        /// <summary>Server password. Set before <see cref="ConnectAsync"/>.</summary>
        public string Password { get; set; } = "password";

        /// <summary>Authentication mode (0 = empty, 1 = skip, 2 = password_protected).</summary>
        public int AuthenticationMode { get; set; } = 2;

        /// <summary>
        /// Routes the internal logger. Unity: <c>(level, msg) => Debug.Log(msg)</c>.
        /// </summary>
        public Action<Logger.LogLevel, string>? LogHandler { get; set; }

        /// <summary>Fired once the client joined a room and is ready to play.</summary>
        public Action? OnReady { get; set; }

        /// <summary>Fired when the client has been torn down.</summary>
        public Action? OnDisconnected { get; set; }

        /// <summary>Fired when another client joins the room.</summary>
        public Action<ulong>? OnRemoteClientAdded { get; set; }

        /// <summary>Fired when another client leaves the room.</summary>
        public Action<ulong>? OnRemoteClientRemoved { get; set; }

        /// <summary>Fired with the latest position of a remote client. Raised from <see cref="Update"/>.</summary>
        public Action<ulong, Position>? OnRemoteClientPosition { get; set; }

        /// <summary>True once the client joined a room and is ready.</summary>
        public bool IsReady { get; private set; }

        /// <summary>The unique id of this client. Valid after <see cref="OnReady"/>.</summary>
        public ulong ClientId { get; private set; }

        /// <summary>The id of the room this client is in. Zero until connected.</summary>
        public ulong ClientRoomId { get; private set; }

        /// <summary>The room this client is in. Valid after <see cref="OnReady"/>.</summary>
        public Room? ClientRoom { get; private set; }

        /// <summary>The client API handle (native pointer wrapper).</summary>
        public ClientAPI? ClientApi { get; private set; }

        /// <summary>Shared logger instance.</summary>
        public NxLogger? Logger { get; private set; }

        /// <summary>Protocol manager (handles packet routing).</summary>
        public ProtocolManager? ProtocolManager { get; private set; }

        /// <summary>TCP transport used for reliable room/player management packets.</summary>
        public BoostTCPClient? TcpClient { get; private set; }

        /// <summary>UDP transport used for latency sensitive packets (positions, movement).</summary>
        public BoostUDPClient? UdpClient { get; private set; }

        readonly HashSet<ulong> _remoteClients = new HashSet<ulong>();
        bool _disposed;

        /// <summary>
        /// Connects to the server, joins the latest active room and becomes
        /// ready. Returns true on success.
        /// </summary>
        public async Task<bool> ConnectAsync()
        {
            if (_disposed) throw new ObjectDisposedException(nameof(NexilisClient));

            NativeInterop.InitializeMainThread();

            Logger = new NxLogger("NexilisClient");
            if (LogHandler != null)
            {
                Logger.Setup(LogHandler);
            }

            ProtocolManager = new ProtocolManager();

            var clientConfig = new ClientConfig();
            clientConfig.SetBoostTCPAddress(ServerAddress);
            clientConfig.SetBoostUDP(ServerAddress);
            clientConfig.SetPassword(Password);
            clientConfig.SetMode(AuthenticationMode);

            ClientApi = new ClientAPI(clientConfig);
            TcpClient = new BoostTCPClient(ProtocolManager, ClientApi);
            UdpClient = new BoostUDPClient(ProtocolManager, ClientApi);

            Logger!.Info("Starting BoostTCP client...");
            bool tcpStarted = await Task.Run(() => TcpClient!.StartClient(ClientApi!));
            if (!tcpStarted)
            {
                throw new InvalidOperationException("BoostTCP client failed to start.");
            }
            Logger.Info("BoostTCP client started.");

            Logger.Info("Starting BoostUDP client...");
            await Task.Run(() => UdpClient!.Start());
            if (!UdpClient!.IsConnected())
            {
                throw new InvalidOperationException("BoostUDP client failed to start.");
            }
            Logger.Info("BoostUDP client started.");

            await WaitForRoomsCreatedAsync();
            await JoinRoomAsync();
            return IsReady;
        }

        async Task WaitForRoomsCreatedAsync()
        {
            using (var waiter = new Waiter(ClientApi!.ClientApiPtr))
            {
                await waiter.WaitUntilRoomsCreated();
            }
            Logger!.Info("Rooms verified and ready.");
        }

        async Task JoinRoomAsync()
        {
            ulong roomId = 0;
            using (var rooms = ClientApi!.GetActiveRooms())
            {
                if (rooms.Count == 0)
                {
                    throw new InvalidOperationException("No active rooms available to join.");
                }

                roomId = rooms.GetRooms().Select(roomPtr => new Room(roomPtr).GetId()).LastOrDefault();
                if (roomId == 0)
                {
                    throw new InvalidOperationException("No active rooms found.");
                }
            }

            Logger!.Info($"Joining room {roomId}...");
            var joinPacket = Packet.RoomManagementJoin(ClientApi.ClientApiPtr, roomId);
            SendTcp(joinPacket);

            await WaitUntilInRoomAsync(roomId);

            ClientRoom = ClientApi.GetRoomFromId(ClientRoomId);
            ClientId = ClientApi.GetClientId();

            Logger!.Info($"Ready. ClientId={ClientId}, room {ClientRoomId} has {ClientRoom!.GetClientAmount()} clients.");
            IsReady = true;
            TryInvoke(OnReady);
        }

        async Task WaitUntilInRoomAsync(ulong expectedRoomId)
        {
            for (int attempt = 0; attempt < 10; attempt++)
            {
                ClientRoomId = ClientApi!.ClientRoomId();
                if (ClientRoomId != 0 && ClientRoomId == expectedRoomId) return;
                await Task.Delay(200);
            }

            throw new InvalidOperationException("Client did not join the room in time.");
        }

        /// <summary>
        /// Polls the room and raises the remote-client callbacks. Call this
        /// once per frame / tick from your game loop (Unity: MonoBehaviour.Update).
        /// </summary>
        public void Update()
        {
            if (_disposed || !IsReady || ClientRoom == null) return;

            var roomClients = ClientRoom.GetClients();
            var present = new HashSet<ulong>();

            foreach (var roomClient in roomClients)
            {
                var id = roomClient.GetId();
                if (id == 0 || id == ClientId) continue;

                present.Add(id);
                if (_remoteClients.Add(id))
                {
                    Logger!.Info($"Remote client added: {id}");
                    TryInvoke(OnRemoteClientAdded!, id);
                }
            }

            foreach (var id in _remoteClients.Where(id => !present.Contains(id)).ToList())
            {
                _remoteClients.Remove(id);
                Logger!.Info($"Remote client removed: {id}");
                TryInvoke(OnRemoteClientRemoved!, id);
            }

            foreach (var roomClient in roomClients)
            {
                var id = roomClient.GetId();
                if (id == 0 || id == ClientId || !_remoteClients.Contains(id)) continue;

                if (ClientSessionNative.nexilis_client_session_get_position_3D_values(
                        roomClient.GetNativePointer(), out float x, out float y, out float z))
                {
                    TryInvoke(OnRemoteClientPosition!, id, new Position(x, y, z));
                }
            }
        }

        /// <summary>
        /// Sends the player position over UDP.
        /// </summary>
        public void SendPosition(float x, float y, float z)
        {
            ThrowIfNotReady();
            var packet = Packet.Player3DPositionDirect(ClientApi!.ClientApiPtr, x, y, z);
            SendUdp(packet);
        }

        /// <summary>
        /// Sends a player movement delta over UDP, which also updates the
        /// position on the server.
        /// </summary>
        public void SendMovement(float x, float y, float z, float deltaTime)
        {
            ThrowIfNotReady();
            var packet = Packet.Player3DMovementDirect(ClientApi!.ClientApiPtr, x, y, z, deltaTime);
            SendUdp(packet);
        }

        /// <summary>
        /// Stops the clients and releases all native resources. Safe to call
        /// multiple times.
        /// </summary>
        public void Disconnect()
        {
            if (_disposed) return;
            _disposed = true;

            IsReady = false;

            SafeDispose(UdpClient);
            SafeDispose(TcpClient);
            SafeDispose(ClientApi);
            SafeDispose(ProtocolManager);
            SafeDispose(Logger);

            UdpClient = null;
            TcpClient = null;
            ClientApi = null;
            ProtocolManager = null;
            Logger = null;
            ClientRoom = null;

            _remoteClients.Clear();
            TryInvoke(OnDisconnected);
        }

        public void Dispose() => Disconnect();

        void SendTcp(NxData data)
        {
            if (data == null) return;

            var bytes = data.ToBytes();
            TcpClient?.SendMessage(bytes, (uint)bytes.Length);
            data.Dispose();
        }

        void SendUdp(NxData data)
        {
            if (data == null) return;

            var bytes = data.ToBytes();
            UdpClient?.SendMessage(bytes, (uint)bytes.Length);
            data.Dispose();
        }

        void ThrowIfNotReady()
        {
            if (_disposed) throw new ObjectDisposedException(nameof(NexilisClient));
            if (!IsReady || ClientApi == null) throw new InvalidOperationException("NexilisClient is not connected.");
        }

        void TryInvoke(Action? callback)
        {
            if (callback == null) return;
            try
            {
                callback();
            }
            catch (Exception ex)
            {
                Logger?.Error($"Exception in callback: {ex.Message}");
            }
        }

        void TryInvoke<T>(Action<T>? callback, T value)
        {
            if (callback == null) return;
            try
            {
                callback(value);
            }
            catch (Exception ex)
            {
                Logger?.Error($"Exception in callback: {ex.Message}");
            }
        }

        void TryInvoke<T1, T2>(Action<T1, T2>? callback, T1 value1, T2 value2)
        {
            if (callback == null) return;
            try
            {
                callback(value1, value2);
            }
            catch (Exception ex)
            {
                Logger?.Error($"Exception in callback: {ex.Message}");
            }
        }

        static void SafeDispose(IDisposable? disposable)
        {
            try
            {
                disposable?.Dispose();
            }
            catch
            {
                // Best effort teardown.
            }
        }
    }
}

