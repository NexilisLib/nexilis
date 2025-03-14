using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class BoostTCPServerNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_create_boost_tcp_server(IntPtr manager, IntPtr settings, int port);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_boost_tcp_server_destroy(IntPtr server);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ProtocolType nexilis_boost_tcp_server_get_type(IntPtr server);
    }
}