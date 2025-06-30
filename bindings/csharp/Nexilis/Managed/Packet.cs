using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class Packet
    {
        static NxLogger _logger = new NxLogger("Packet");
        public static void InitializeLogger(Action<Logger.LogLevel, string> logCallback) => _logger.Setup(logCallback);

        public static NxData InfoRooms()
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_info_rooms();
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to get packet info rooms.");
                }
                return NxData.Create(raw);
            }, "InfoRooms");
        }

        public static NxData RoomManagementCreate(RoomContext context, string roomName)
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_room_management_create(context, roomName);
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to get room management create.");
                }
                return NxData.Create(raw);
            }, "RoomManagementCreate");
        }
        public static NxData RoomManagementJoin(ulong roomId)
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_room_management_join(roomId);
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to get room management join.");
                }
                return NxData.Create(raw);
            }, "RoomManagementJoin");
        }

        public static NxData Player3DPosition(Vector3<float> position)
        {
            // TODO fix this
            return NativeInterop.ExecuteSafe(() =>
            {
                return Packet.Player3DPositionDirect(position.X, position.Y, position.Z);
            }, "Player3DPosition");
        }
        public static NxData Player3DPositionDirect(float x, float y, float z)
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = PacketNative.nexilis_packet_room_player3D_position_direct(x, y, z);
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Native call returned null");
                }
                return NxData.Create(raw);
            }, "Player3DPositionDirect");
        }
    }
}
