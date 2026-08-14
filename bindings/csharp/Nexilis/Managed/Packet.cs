using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class Packet
    {
        static NxLogger _logger = new NxLogger("Packet");
        public static void InitializeLogger(Action<Logger.LogLevel, string> logCallback) => _logger.Setup(logCallback);

        public static NxData InfoRooms(IntPtr clientApi)
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_get_info_rooms(clientApi);
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to get packet info rooms.");
                }
                return NxData.Create(raw);
            }, "InfoRooms");
        }

        public static NxData RoomManagementCreate(IntPtr clientApi, RoomContext context, string roomName)
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_room_management_create(clientApi, roomName, context);
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to get room management create.");
                }
                return NxData.Create(raw);
            }, "RoomManagementCreate");
        }
        public static NxData RoomManagementJoin(IntPtr clientApi, ulong roomId)
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_room_management_join(clientApi, roomId);
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to get room management join.");
                }
                return NxData.Create(raw);
            }, "RoomManagementJoin");
        }

        public static NxData Player3DPosition(IntPtr clientApi, Vector3<float> position)
        {
            // TODO fix this
            return NativeInterop.ExecuteSafe(() =>
            {
                return Packet.Player3DPositionDirect(clientApi, position.X, position.Y, position.Z);
            }, "Player3DPosition");
        }
        public static NxData Player3DPositionDirect(IntPtr clientApi, float x, float y, float z)
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_room_player3D_position_direct(clientApi, x, y, z);
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Native call returned null");
                }
                return NxData.Create(raw);
            }, "Player3DPositionDirect");
        }
        public static NxData Player3DMovement(IntPtr clientApi, Vector3<float> movement, float deltatime)
        {
            return Player3DMovementDirect(clientApi, movement.X, movement.Y, movement.Z, deltatime);
        }

        public static NxData Player3DMovementDirect(IntPtr clientApi, float x, float y, float z, float deltatime)
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_room_player3D_movement_direct(clientApi, x, y, z, deltatime);
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to call nexilis_packet_room_player3D_movement_direct");
                }
                return NxData.Create(raw);
            }, "Player3DMovementDirect");
        }
    }
}
