using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RawVector3
    {
        public IntPtr vec;

        public bool IsValid => vec != IntPtr.Zero;

        public void Destroy()
        {
            if (IsValid)
            {
                Vector3Native.nexilis_vector3f_destroy(vec);
                vec = IntPtr.Zero;
            }
        }
    }

    public static class Vector3Native
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_vector3f_create(float x, float y, float z);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_vector3f_create_default();
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_destroy(IntPtr vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern float nexilis_vector3f_get_x(IntPtr vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern float nexilis_vector3f_get_y(IntPtr vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern float nexilis_vector3f_get_z(IntPtr vec);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_set_x(IntPtr vec, float x);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_set_y(IntPtr vec, float y);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_set_z(IntPtr vec, float z);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_vector3f_serialize(IntPtr vec, byte[] out_data);
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_vector3f_deserialize(byte[] data);
    }

}
