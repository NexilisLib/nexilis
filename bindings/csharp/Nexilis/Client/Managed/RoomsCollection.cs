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

        public IEnumerable<IntPtr> GetRooms()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(RoomsCollection));

            for (ulong i = 0; i < Count; i++)
            {
                yield return RoomsCollectionNative.GetRoom(_nativeCollection, i);
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

