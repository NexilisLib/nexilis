using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class RoomNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_room_get_id(IntPtr room);
    }
}