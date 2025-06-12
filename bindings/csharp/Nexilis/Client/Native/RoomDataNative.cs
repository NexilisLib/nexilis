using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public class RoomDataNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_room_data_create(
            ulong creatorId,
            [MarshalAs(UnmanagedType.LPStr)] string name,
            RoomContext context,
            uint maxSize);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_room_data_destroy(IntPtr roomData);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_room_data_get_name(IntPtr roomData);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RoomContext nexilis_room_data_get_context(IntPtr roomData);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint nexilis_room_data_get_max_size(IntPtr roomData);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_room_data_get_id(IntPtr roomData);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_room_data_get_creator_id(IntPtr roomData);
    }
}
