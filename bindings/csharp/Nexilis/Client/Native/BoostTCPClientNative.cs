using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    class BoostTCPClientNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_boost_tcp_client_create(IntPtr protocolManager, IntPtr clientApi);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_boost_tcp_client_destroy(IntPtr client);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ProtocolType nexilis_boost_tcp_client_get_type(IntPtr client);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_boost_tcp_client_start(IntPtr client);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_boost_tcp_client_stop(IntPtr client);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_boost_tcp_client_send_message(IntPtr client, byte[] message, uint length);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_boost_tcp_client_send_message_with_callback(
                IntPtr client, byte[] message, uint length, IntPtr callback);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool nexilis_start_client(IntPtr clientApi, IntPtr tcpClient);
    }
}

