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
using System.Collections.Generic;

namespace Nexilis.Client
{
    public class RoomsCollection : IDisposable
    {
        private readonly IntPtr _nativeCollection;
        private bool _disposed = false;

        public RoomsCollection(IntPtr nativeCollection)
        {
            if (nativeCollection == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(nativeCollection));
            }
            _nativeCollection = nativeCollection;
            Count = RoomsCollectionNative.GetCount(_nativeCollection);
        }

        public ulong Count { get; }

        /// <summary>True when the collection holds no rooms.</summary>
        public bool IsEmpty => Count == 0;

        /// <summary>
        /// Gets a non-owning wrapper for the room at <paramref name="index"/>.
        /// </summary>
        /// <remarks>
        /// The native side hands out a pointer to a shared, reused wrapper, so
        /// the returned room is only valid until the next index is requested
        /// and must not be disposed. Use <see cref="GetRoomInfos"/> when you
        /// need values that stay valid after the collection is gone.
        /// </remarks>
        /// <exception cref="ArgumentOutOfRangeException">Thrown when index is out of range.</exception>
        /// <exception cref="ObjectDisposedException">Thrown when the collection has been disposed.</exception>
        public Room GetRoom(ulong index)
        {
            ThrowIfDisposed();
            if (index >= Count)
            {
                throw new ArgumentOutOfRangeException(nameof(index), index, $"Collection holds {Count} room(s).");
            }

            var ptr = RoomsCollectionNative.GetRoom(_nativeCollection, index);
            if (ptr == IntPtr.Zero)
            {
                throw new InvalidOperationException($"Failed to get room at index {index}.");
            }

            return new Room(ptr, ownsNativeInstance: false);
        }

        /// <summary>
        /// Gets a non-owning wrapper for every room in the collection. All
        /// wrappers share the same native handle, so read what you need before
        /// requesting another room and never dispose the wrappers.
        /// </summary>
        public IReadOnlyList<Room> GetRooms()
        {
            ThrowIfDisposed();

            var rooms = new List<Room>((int)Count);
            for (ulong i = 0; i < Count; i++)
            {
                var ptr = RoomsCollectionNative.GetRoom(_nativeCollection, i);
                if (ptr != IntPtr.Zero)
                {
                    rooms.Add(new Room(ptr, ownsNativeInstance: false));
                }
            }
            return rooms.AsReadOnly();
        }

        /// <summary>
        /// Copies the metadata of every room into <see cref="RoomInfo"/> values
        /// that stay valid after this collection has been disposed.
        /// </summary>
        public IReadOnlyList<RoomInfo> GetRoomInfos()
        {
            ThrowIfDisposed();

            var infos = new List<RoomInfo>((int)Count);
            for (ulong i = 0; i < Count; i++)
            {
                var ptr = RoomsCollectionNative.GetRoom(_nativeCollection, i);
                if (ptr == IntPtr.Zero)
                {
                    continue;
                }

                // Read everything out of the shared wrapper right away, the
                // next GetRoom call overwrites it.
                using (var room = new Room(ptr, ownsNativeInstance: false))
                {
                    infos.Add(RoomInfo.FromRoom(room));
                }
            }
            return infos.AsReadOnly();
        }

        void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(RoomsCollection));
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_nativeCollection != IntPtr.Zero)
                {
                    RoomsCollectionNative.Free(_nativeCollection);
                }
                _disposed = true;
            }
        }

        ~RoomsCollection()
        {
            Dispose(false);
        }
    }
}
