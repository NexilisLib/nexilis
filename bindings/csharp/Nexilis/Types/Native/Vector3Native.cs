using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RawVector3
    {
        public IntPtr vector;
    }

    public static class Vector3Native
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawVector3 nexilis_vector3f_create(float x, float y, float z);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawVector3 nexilis_vector3f_create_default();
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_destroy(RawVector3 vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern float nexilis_vector3f_get_x(RawVector3 vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern float nexilis_vector3f_get_y(RawVector3 vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern float nexilis_vector3f_get_z(RawVector3 vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_set_x(RawVector3 vec, float x);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_set_y(RawVector3 vec, float y);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_set_z(RawVector3 vec, float z);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_serialize(RawVector3 vec, byte[] out_data);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawVector3 nexilis_vector3f_deserialize(byte[] data);
    }

}
