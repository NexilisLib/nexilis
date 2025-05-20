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