using System;
using System.Runtime.InteropServices;

namespace Nexilis.Logger
{
    public static class FileHandlerNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_logger_FileHandler_create(string filename);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_logger_FileHandler_destroy(IntPtr handler);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_logger_FileHandler_get_id(IntPtr handler);
    }
}

