namespace Nexilis.Client
{
    public class ClientSession : IDisposable
    {
        IntPtr _nativePointer;
        bool _disposed = false;

        public ClientSession(ulong id, IntPtr clientApiHandle)
        {
            if (clientApiHandle == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(clientApiHandle));
            }
            _nativePointer = ClientSessionNative.nexilis_client_session_create(id, clientApiHandle);
        }

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
