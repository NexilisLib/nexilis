using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public sealed class NxData : IDisposable
    {
        private static NxLogger _logger = new NxLogger("NxData");
        public static void InitializeLogger(Action<Logger.LogLevel, string> logCallback)
        {
            _logger.AddHandler(logCallback);
            _logger.SetMinimumLevel(Logger.LogLevel.DEBUG);
        }
        private RawNxData _rawData;
        private bool _disposed = false;

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
