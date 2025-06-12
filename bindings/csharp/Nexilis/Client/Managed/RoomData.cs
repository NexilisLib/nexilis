using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public class RoomData : IDisposable
    {
        IntPtr _nativePtr;
        bool _disposed = false;

        public RoomData(ulong creatorId, string name, RoomContext context, uint maxSize)
        {
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentException(nameof(name));
            }
            _nativePtr = RoomDataNative.nexilis_room_data_create(creatorId, name, context, maxSize);
        }

        public string Name
        {
            get
            {
                ThrowIfDisposed();
                IntPtr namePtr = RoomDataNative.nexilis_room_data_get_name(_nativePtr);
                return Marshal.PtrToStringAnsi(namePtr);
            }
        }

        public RoomContext Context
        {
            get
            {
                ThrowIfDisposed();
                return RoomDataNative.nexilis_room_data_get_context(_nativePtr);
            }
        }

        public uint MaxSize
        {
            get
            {
                ThrowIfDisposed();
                return RoomDataNative.nexilis_room_data_get_max_size(_nativePtr);
            }
        }

        public ulong Id
        {
            get
            {
                ThrowIfDisposed();
                return RoomDataNative.nexilis_room_data_get_id(_nativePtr);
            }
        }

        public ulong CreatorId
        {
            get
            {
                ThrowIfDisposed();
                return RoomDataNative.nexilis_room_data_get_creator_id(_nativePtr);
            }
        }

        public IntPtr NativePointer
        {
            get
            {
                ThrowIfDisposed();
                return _nativePtr;
            }
        }

        void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(RoomData));
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

        ~RoomData()
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
