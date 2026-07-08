using System;
using System.Runtime.InteropServices;

namespace Nexilis.Server
{
    public static class ServerConfigNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_server_config_create();

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_config_destroy(IntPtr config);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_config_set_passphrase(IntPtr config, string password);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool nexilis_server_config_is_passphrase(IntPtr config, string password);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool nexilis_server_config_has_passphrase(IntPtr config);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_server_config_get_passphrase(IntPtr config);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_config_set_root_password(IntPtr config, string password);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool nexilis_server_config_is_root_password(IntPtr config, string password);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool nexilis_server_config_has_root_password(IntPtr config);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_server_config_get_root_password(IntPtr config);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_config_set_mode(IntPtr config, AuthenticationMode mode);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern AuthenticationMode nexilis_server_config_get_mode(IntPtr config);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_server_config_set_tickrate(IntPtr config, float tickrate);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern float nexilis_server_config_get_tickrate(IntPtr config);
    }
}
