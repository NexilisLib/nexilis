using System;
using System.Runtime.InteropServices;

namespace Nexilis.Server
{
    public class Settings : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed = false;

        public Settings()
        {
            _handle = SettingsNative.nexilis_settings_create();
            if (_handle == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create Settings.");
            }
        }

        ~Settings()
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
                    SettingsNative.nexilis_settings_destroy(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        public void SetPassphrase(string password)
        {
            ThrowIfDisposed();
            SettingsNative.nexilis_settings_set_passphrase(_handle, password);
        }

        public bool IsPassphrase(string password)
        {
            ThrowIfDisposed();
            return SettingsNative.nexilis_settings_is_passphrase(_handle, password);
        }

        public bool HasPassphrase()
        {
            ThrowIfDisposed();
            return SettingsNative.nexilis_settings_has_passphrase(_handle);
        }

        public string GetPassphrase()
        {
            ThrowIfDisposed();
            IntPtr passphrasePtr = SettingsNative.nexilis_settings_get_passphrase(_handle);
            return Marshal.PtrToStringAnsi(passphrasePtr) ?? string.Empty;
        }

        // Root password
        public void SetRootPassword(string password)
        {
            ThrowIfDisposed();
            SettingsNative.nexilis_settings_set_root_password(_handle, password);
        }

        public bool IsRootPassword(string password)
        {
            ThrowIfDisposed();
            return SettingsNative.nexilis_settings_is_root_password(_handle, password);
        }

        public bool HasRootPassword()
        {
            ThrowIfDisposed();
            return SettingsNative.nexilis_settings_has_root_password(_handle);
        }

        public string GetRootPassword()
        {
            ThrowIfDisposed();
            IntPtr rootPasswordPtr = SettingsNative.nexilis_settings_get_root_password(_handle);
            return Marshal.PtrToStringAnsi(rootPasswordPtr) ?? string.Empty;
        }

        public void SetMode(AuthenticationMode mode)
        {
            ThrowIfDisposed();
            SettingsNative.nexilis_settings_set_mode(_handle, mode);
        }

        public AuthenticationMode GetMode()
        {
            ThrowIfDisposed();
            return SettingsNative.nexilis_settings_get_mode(_handle);
        }

        public void SetTickrate(float tickrate)
        {
            ThrowIfDisposed();
            SettingsNative.nexilis_settings_set_tickrate(_handle, tickrate);
        }

        public float GetTickrate()
        {
            ThrowIfDisposed();
            return SettingsNative.nexilis_settings_get_tickrate(_handle);
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(Settings));
            }
        }
    }
}