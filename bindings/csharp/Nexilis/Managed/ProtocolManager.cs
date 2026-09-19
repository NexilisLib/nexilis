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
using System.Runtime.InteropServices;

namespace Nexilis
{
    public class ProtocolManager : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed = false;

        public ProtocolManager()
        {
            _handle = ProtocolManagerNative.nexilis_protocol_manager_create();
            if (_handle == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create ProtocolManager.");
            }
        }

        public IntPtr ProtocolManagerPtr => _handle;

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
                    ProtocolManagerNative.nexilis_protocol_manager_destroy(_handle);
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
                _handle = ProtocolManagerNative.nexilis_protocol_data_create(type);
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
                        ProtocolManagerNative.nexilis_protocol_data_destroy(_handle);
                        _handle = IntPtr.Zero;
                    }
                    _disposed = true;
                }
            }

            public ProtocolType GetProtocolType()
            {
                ThrowIfDisposed();
                return ProtocolManagerNative.nexilis_protocol_data_get_type(_handle);
            }

            public ProtocolStatus GetStatus()
            {
                ThrowIfDisposed();
                return ProtocolManagerNative.nexilis_protocol_data_get_status(_handle);
            }

            public ulong GetId()
            {
                ThrowIfDisposed();
                return ProtocolManagerNative.nexilis_protocol_data_get_id(_handle);
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

