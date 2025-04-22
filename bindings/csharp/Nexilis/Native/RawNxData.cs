using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RawNxData
    {
        public IntPtr data;
        public UIntPtr size;
    }

    public static class RawNxDataNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_nx_data_create(UIntPtr size);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_nx_data_create_from(IntPtr data, UIntPtr size);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_nx_data_destroy(IntPtr data);
    }
}