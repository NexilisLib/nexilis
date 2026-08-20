using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class PacketNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_get_general_clientId(IntPtr clientApi);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_get_info_general(IntPtr clientApi);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_get_info_clients(IntPtr clientApi);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_get_info_rooms(IntPtr clientApi);


        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_room_management_join(IntPtr clientApi, ulong roomId);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_room_management_leave(IntPtr clientApi);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_room_management_create(IntPtr clientApi, string roomName, RoomContext context);


        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_room_player3D_position(IntPtr clientApi, IntPtr vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_room_player3D_position_direct(IntPtr clientApi, float x, float y, float z);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_room_player3D_dimension(IntPtr clientApi, IntPtr vector);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_room_player3D_movement(IntPtr clientApi, IntPtr vector, float deltaTime);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_room_player3D_movement_direct(IntPtr clientApi,
                float x, float y, float z, float deltatime);
    }

}
