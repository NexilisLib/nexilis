using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class ClientAPINative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_client_api_create(IntPtr serverData);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_api_destroy(IntPtr client);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_client_api_is_boost_tcp_ready(IntPtr client_api);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_client_api_is_boost_udp_ready(IntPtr client_api);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_client_api_is_initialized(IntPtr client_api);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_api_wait_until_boost_tcp_ready(IntPtr client_api);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_api_wait_until_boost_udp_ready(IntPtr client_api);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_client_api_get_boost_tcp_server_address(IntPtr client_api);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_client_api_get_boost_udp_server_address(IntPtr client_api);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_client_api_get_client_id(IntPtr client_api);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_client_api_get_client_username(IntPtr client_api);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern string nexilis_client_api_get_client_password(IntPtr client_api);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_client_api_get_active_rooms(IntPtr client_api);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_client_api_rooms_count(IntPtr client_api);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_client_api_client_room_id(IntPtr client_api);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_client_api_get_room(IntPtr client_api, ulong room_id);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_client_api_get_client_from_room(IntPtr client_api, ulong client_id);
    }
}
