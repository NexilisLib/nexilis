using System;
using System.Runtime.InteropServices;

namespace Nexilis.Logger
{
    public static class FunctionHandlerNative
    {
        // Delegate for the native callback function.
        public delegate void NativeLogCallback(LogLevel logLevel, IntPtr message);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_logger_FunctionHandler_create(NativeLogCallback callback);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_FunctionHandler_destroy(IntPtr handler);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_logger_FunctionHandler_get_id(IntPtr handler);
    }
}

