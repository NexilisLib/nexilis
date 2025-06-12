using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public class ClientSessionNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_client_session_create(ulong id, IntPtr clientApi);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_session_destroy(IntPtr client);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_client_session_move(IntPtr client);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_session_move_assing(IntPtr dest, IntPtr src);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_client_session_equal(IntPtr lhs, IntPtr rhs);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool nexilis_client_session_not_equal(IntPtr lhs, IntPtr rhs);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_client_session_set_username(IntPtr session, string new_username);
    }
}
