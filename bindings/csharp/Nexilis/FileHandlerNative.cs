using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class FileHandlerNative
    {
        private const string NativeLibrary = "nexilis";

        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_logger_FileHandler_create(string filename);

        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_FileHandler_destroy(IntPtr handler);
        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_logger_FileHandler_get_id(IntPtr handler);
    }
}