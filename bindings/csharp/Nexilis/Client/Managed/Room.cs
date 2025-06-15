using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public class Room : IDisposable
    {
        IntPtr _nativePtr;
        bool _disposed = false;

        // TODO add clients to constructor.
        public Room(RoomData roomData, List<ClientSession> clients)
        {
            if (roomData.NativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(roomData));
            }
            _nativePtr = RoomNative.nexilis_room_create(roomData.NativePointer, IntPtr.Zero);
        }

        public Room(IntPtr nativePointer)
        {
            if (nativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(nativePointer));
            }
            _nativePtr = nativePointer;
        }

        public ulong GetClientAmount()
        {
            return RoomNative.nexilis_room_get_client_amount(_nativePtr);
        }

        public ulong GetId()
        {
            return RoomNative.nexilis_room_get_id(_nativePtr);
        }

        public void AddClient(ClientSession client)
        {
            ThrowIfDisposed();
            if (client == null)
                throw new ArgumentNullException(nameof(client));

            RoomNative.nexilis_room_add_client(_nativePtr, client.GetNativePointer());
        }

        public void RemoveClient(ulong clientId)
        {
            ThrowIfDisposed();
            RoomNative.nexilis_room_remove_client(_nativePtr, clientId);
        }

        void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(Room));
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_nativePtr != IntPtr.Zero)
                {
                    RoomDataNative.nexilis_room_data_destroy(_nativePtr);
                    _nativePtr = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        ~Room()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}
