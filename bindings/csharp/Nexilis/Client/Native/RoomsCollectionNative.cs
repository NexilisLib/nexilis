using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class RoomsCollectionNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        private static extern ulong nexilis_rooms_collection_rooms_count(IntPtr collection);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr nexilis_rooms_collection_rooms_get(IntPtr collection, ulong index);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        private static extern void nexilis_rooms_collection_free(IntPtr collection);

        public static ulong GetCount(IntPtr collection)
        {
            if (collection == IntPtr.Zero)
                throw new ArgumentNullException(nameof(collection));
            
            return nexilis_rooms_collection_rooms_count(collection);
        }

        public static IntPtr GetRoom(IntPtr collection, ulong index)
        {
            if (collection == IntPtr.Zero)
                throw new ArgumentNullException(nameof(collection));
            
            return nexilis_rooms_collection_rooms_get(collection, index);
        }

        public static void Free(IntPtr collection)
        {
            if (collection != IntPtr.Zero)
            {
                nexilis_rooms_collection_free(collection);
            }
        }
    }
}