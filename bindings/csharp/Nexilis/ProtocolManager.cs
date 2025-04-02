using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public class ProtocolManager : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed = false;

        public ProtocolManager()
        {
            _handle = ProtocolManagerNative.nexilis_ProtocolManager_create();
            if (_handle == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create ProtocolManager.");
            }
        }

        ~ProtocolManager()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public IntPtr GetHandle()
        {
            ThrowIfDisposed();
            return _handle;
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_handle != IntPtr.Zero)
                {
                    ProtocolManagerNative.nexilis_ProtocolManager_destroy(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(ProtocolManager));
            }
        }

        public class ProtocolData : IDisposable
        {
            private IntPtr _handle;
            private bool _disposed = false;

            public ProtocolData(ProtocolType type)
            {
                _handle = ProtocolManagerNative.nexilis_ProtocolData_create(type);
                if (_handle == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to create ProtocolData.");
                }
            }

            ~ProtocolData()
            {
                Dispose(false);
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
                    if (_handle != IntPtr.Zero)
                    {
                        ProtocolManagerNative.nexilis_ProtocolData_destroy(_handle);
                        _handle = IntPtr.Zero;
                    }
                    _disposed = true;
                }
            }

            public ProtocolType GetProtocolType()
            {
                ThrowIfDisposed();
                return ProtocolManagerNative.nexilis_ProtocolData_get_type(_handle);
            }

            public ProtocolStatus GetStatus()
            {
                ThrowIfDisposed();
                return ProtocolManagerNative.nexilis_ProtocolData_get_status(_handle);
            }

            public ulong GetId()
            {
                ThrowIfDisposed();
                return ProtocolManagerNative.nexilis_ProtocolData_get_id(_handle);
            }

            private void ThrowIfDisposed()
            {
                if (_disposed)
                {
                    throw new ObjectDisposedException(nameof(ProtocolData));
                }
            }
        }
    }
}