using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class ServerDataNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_server_data_create();

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_data_destroy(IntPtr serverData);


        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_data_set_password(IntPtr serverData, string password);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_server_data_get_password(IntPtr serverData);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_data_set_boost_tcp_address(IntPtr serverData, string address);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_server_data_get_boost_tcp_server_address(IntPtr serverData);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_data_set_boost_udp(IntPtr serverData, string address);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_server_data_get_boost_udp_server_adress(IntPtr serverData);
    }
}
