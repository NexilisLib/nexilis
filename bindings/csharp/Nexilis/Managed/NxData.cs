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
    public sealed class NxData : IDisposable
    {
        static NxLogger _logger = new NxLogger("NxData");
        public static void InitializeLogger(Action<Logger.LogLevel, string> logCallback) => _logger.Setup(logCallback);

        RawNxData _rawData;
        bool _disposed = false;

        public ulong Size
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException(nameof(NxData));
                return RawNxDataNative.nexilis_nx_data_get_size(ref _rawData);
            }
        }
        public IntPtr DataPointer
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException(nameof(NxData));
                return RawNxDataNative.nexilis_nx_data_get_data(ref _rawData);
            }
        }

        internal NxData(RawNxData rawData)
        {
            _rawData = rawData;
        }

        public static NxData Create(RawNxData rawData)
        {
            if (rawData.data == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(rawData));
            }
            try
            {
                var size = RawNxDataNative.nexilis_nx_data_get_size(ref rawData);
                if (size == 0) throw new InvalidOperationException("Invalid data size");
            }
            catch (Exception ex)
            {
                throw new ArgumentException("Invalid RawNxData", nameof(rawData), ex);
            }
            return new NxData(rawData);
        }

        public byte[] ToBytes()
        {
            if (_disposed) throw new ObjectDisposedException(nameof(NxData));

            var size = Size;
            if (size == 0) return Array.Empty<byte>();

            var dataPtr = RawNxDataNative.nexilis_nx_data_get_data(ref _rawData);
            if (dataPtr == IntPtr.Zero) return Array.Empty<byte>();

            byte[] bytes = new byte[(int)size];
            Marshal.Copy(dataPtr, bytes, 0, (int)size);
            return bytes;
        }

        public void Dispose()
        {
            if (_disposed) return;

            if (_rawData.data != IntPtr.Zero)
            {
                RawNxDataNative.nexilis_nx_data_destroy(ref _rawData);
                _rawData.data = IntPtr.Zero;
            }

            _disposed = true;
            GC.SuppressFinalize(this);
        }

        ~NxData() => Dispose();
    }
}
