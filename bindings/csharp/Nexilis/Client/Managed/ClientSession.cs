namespace Nexilis.Client
{
    public class ClientSession : IDisposable
    {
        IntPtr _nativePointer;
        bool _disposed = false;
        ulong _id;

        public ClientSession(ulong id, IntPtr clientApiHandle)
        {
            if (clientApiHandle == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(clientApiHandle));
            }
            _id = id;
            _nativePointer = ClientSessionNative.nexilis_client_session_create(_id, clientApiHandle);
        }

        public ulong GetId() => _id;

        public IntPtr GetNativePointer() => _nativePointer;

        // TODO implement move semantics

        public void SetUsername(string username)
        {
            ThrowIfDisposed();
            ClientSessionNative.nexilis_client_session_set_username(_nativePointer, username);
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
                if (_nativePointer != IntPtr.Zero)
                {
                    ClientSessionNative.nexilis_client_session_destroy(_nativePointer);
                    _nativePointer = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        ~ClientSession()
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
