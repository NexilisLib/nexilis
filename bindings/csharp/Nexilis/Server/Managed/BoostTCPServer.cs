using System;
using System.Runtime.InteropServices;

namespace Nexilis.Server
{
    public class BoostTCPServer : IDisposable
    {
        private IntPtr _handle;
        private Settings _settings;
        private bool _disposed = false;

        public BoostTCPServer(ProtocolManager protocolManager, Settings settings, int port)
        {
            NullCheck.ThrowIfNull(protocolManager, nameof(protocolManager));
            NullCheck.ThrowIfNull(settings, nameof(settings));

            this._settings = settings;

            _handle = BoostTCPServerNative.nexilis_create_boost_tcp_server(
                protocolManager.GetHandle(), 
                this._settings.GetHandle(), 
                port
            );

            if (_handle == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create BoostTCPServer.");
            }
        }

        ~BoostTCPServer()
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
                    BoostTCPServerNative.nexilis_boost_tcp_server_destroy(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        public Settings GetSettings()
        {
            ThrowIfDisposed();
            return _settings;
        }

        public ProtocolType GetProtocolType()
        {
            ThrowIfDisposed();
            return BoostTCPServerNative.nexilis_boost_tcp_server_get_type(_handle);
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(BoostTCPServer));
            }
        }

        internal IntPtr GetHandle()
        {
            return _handle;
        }
    }
}