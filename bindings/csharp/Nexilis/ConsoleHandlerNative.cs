using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class ConsoleHandlerNative
    {
        private const string NativeLibrary = "nexilis";

        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_logger_ConsoleHandler_create();

        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_ConsoleHandler_destroy(IntPtr handler);
        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_logger_ConsoleHandler_get_id(IntPtr handler);
    }
}