using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class Packet
    {
        private static NxLogger _logger = new NxLogger("Packet");
        public static void InitializeLogger(Action<Logger.LogLevel, string> logCallback)
        {
            _logger.AddHandler(logCallback);
            _logger.SetMinimumLevel(Logger.LogLevel.DEBUG);
        }

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
            return NativeInterop.ExecuteSafe(() =>
            {
                var nativePointer = position.getNative();
                if (nativePointer.vector == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Nativepointer fails");
                }
                var raw = PacketNative.nexilis_packet_room_player3D_position(position.getNative());
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to get room management join.");
                }
                return NxData.Create(raw);
            }, "Player3DPosition");
        }
    }
}
