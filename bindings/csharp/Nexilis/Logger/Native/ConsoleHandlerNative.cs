using System;
using System.Runtime.InteropServices;

namespace Nexilis.Logger
{
    public static class ConsoleHandlerNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_logger_ConsoleHandler_create();
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_ConsoleHandler_destroy(IntPtr handler);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_logger_ConsoleHandler_get_id(IntPtr handler);
    }
}

