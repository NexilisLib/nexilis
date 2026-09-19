 /* Copyright (C) 2026 Valtteri Viirret
    This file is part of the Nexilis Project.

    This file is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This file is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this file.  If not, see <https://gnu.org>. */

using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class RoomsCollectionNative
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        private static extern ulong nexilis_rooms_collection_rooms_count(IntPtr collection);

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr nexilis_rooms_collection_room_get(IntPtr collection, ulong index);

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

            return nexilis_rooms_collection_room_get(collection, index);
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
