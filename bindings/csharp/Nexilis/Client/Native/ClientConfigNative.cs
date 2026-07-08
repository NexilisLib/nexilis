using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class ClientConfigNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_client_config_create();

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_config_destroy(IntPtr config);


        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_config_set_password(IntPtr config, string password);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_client_config_get_password(IntPtr config);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_config_set_boost_tcp_address(IntPtr config, string address);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_client_config_get_boost_tcp_server_address(IntPtr config);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_config_set_boost_udp_address(IntPtr config, string address);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_client_config_get_boost_udp_server_address(IntPtr config);
    }
}
