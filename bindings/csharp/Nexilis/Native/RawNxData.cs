using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RawNxData
    {
        public IntPtr data;
    }

    public static class RawNxDataNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_nx_data_create(ulong size);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_nx_data_create_from(IntPtr data, ulong size);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_nx_data_destroy(ref RawNxData handle);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong nexilis_nx_data_get_size(ref RawNxData handle);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_nx_data_get_data(ref RawNxData handle);
    }
}

