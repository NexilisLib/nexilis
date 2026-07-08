using System;
using System.Runtime.InteropServices;

namespace Nexilis.Server
{
    public class ServerConfig : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed = false;

        public ServerConfig()
        {
            _handle = ServerConfigNative.nexilis_server_config_create();
            if (_handle == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create ServerConfig.");
            }
        }

        ~ServerConfig()
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
                    ServerConfigNative.nexilis_server_config_destroy(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        public void SetPassphrase(string password)
        {
            ThrowIfDisposed();
            ServerConfigNative.nexilis_server_config_set_passphrase(_handle, password);
        }

        public bool IsPassphrase(string password)
        {
            ThrowIfDisposed();
            return ServerConfigNative.nexilis_server_config_is_passphrase(_handle, password);
        }

        public bool HasPassphrase()
        {
            ThrowIfDisposed();
            return ServerConfigNative.nexilis_server_config_has_passphrase(_handle);
        }

        public string GetPassphrase()
        {
            ThrowIfDisposed();
            IntPtr passphrasePtr = ServerConfigNative.nexilis_server_config_get_passphrase(_handle);
            return Marshal.PtrToStringAnsi(passphrasePtr) ?? string.Empty;
        }

        // Root password
        public void SetRootPassword(string password)
        {
            ThrowIfDisposed();
            ServerConfigNative.nexilis_server_config_set_root_password(_handle, password);
        }

        public bool IsRootPassword(string password)
        {
            ThrowIfDisposed();
            return ServerConfigNative.nexilis_server_config_is_root_password(_handle, password);
        }

        public bool HasRootPassword()
        {
            ThrowIfDisposed();
            return ServerConfigNative.nexilis_server_config_has_root_password(_handle);
        }

        public string GetRootPassword()
        {
            ThrowIfDisposed();
            IntPtr rootPasswordPtr = ServerConfigNative.nexilis_server_config_get_root_password(_handle);
            return Marshal.PtrToStringAnsi(rootPasswordPtr) ?? string.Empty;
        }

        public void SetMode(AuthenticationMode mode)
        {
            ThrowIfDisposed();
            ServerConfigNative.nexilis_server_config_set_mode(_handle, mode);
        }

        public AuthenticationMode GetMode()
        {
            ThrowIfDisposed();
            return ServerConfigNative.nexilis_server_config_get_mode(_handle);
        }

        public void SetTickrate(float tickrate)
        {
            ThrowIfDisposed();
            ServerConfigNative.nexilis_server_config_set_tickrate(_handle, tickrate);
        }

        public float GetTickrate()
        {
            ThrowIfDisposed();
            return ServerConfigNative.nexilis_server_config_get_tickrate(_handle);
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(ServerConfig));
            }
        }
    }
}
