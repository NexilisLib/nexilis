using System.Runtime.InteropServices;

namespace Nexilis
{
    public static class FunctionHandlerNative
    {
        private const string NativeLibrary = "nexilis";
        
        // Delegate for the native callback function.
        public delegate void NativeLogCallback(LogLevel logLevel, IntPtr message);

        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_logger_FunctionHandler_create(NativeLogCallback callback);

        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_FunctionHandler_destroy(IntPtr handler);

        [DllImport(NativeLibrary, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_logger_FunctionHandler_get_id(IntPtr handler);
    }
}