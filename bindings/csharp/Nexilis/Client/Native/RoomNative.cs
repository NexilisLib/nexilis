using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class RoomNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_room_create(IntPtr roomData, IntPtr clients);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_room_destroy(IntPtr room);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_room_get_id(IntPtr room);
    }
}
