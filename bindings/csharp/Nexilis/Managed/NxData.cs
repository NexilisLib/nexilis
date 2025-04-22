using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public sealed class NxData : IDisposable
    {
        private IntPtr _handle;
        private RawNxData _rawData;
        private bool _disposed = false;

        public IntPtr Data => _rawData.data;
        public int Size => (int)_rawData.size;

        internal NxData(IntPtr handle, RawNxData rawData)
        {
            _handle = handle;
            _rawData = rawData;
        }

        public static NxData Create(int size)
        {
            if (size < 0)
            {
                throw new ArgumentOutOfRangeException(nameof(size));
            }
            IntPtr handle = RawNxDataNative.nexilis_nx_data_create((UIntPtr)size);
            return new NxData(handle, new RawNxData {
                data = handle,
                size = (UIntPtr)size
            });
        }

        public static NxData FromHandle(IntPtr handle)
        {
            if (handle == IntPtr.Zero)
                throw new ArgumentNullException(nameof(handle));

            return new NxData(handle, new RawNxData {
                data = handle,
                size = (UIntPtr)0 // TODO
            });
        }

        public static NxData FromBytes(byte[] data)
        {
            if (data == null || data.Length == 0)
            {
                throw new ArgumentException("Data cannot be null or empty.", nameof(data));
            }

            IntPtr unmanagedArray = Marshal.AllocHGlobal(data.Length);
            Marshal.Copy(data, 0, unmanagedArray, data.Length);
            
            IntPtr handle = RawNxDataNative.nexilis_nx_data_create_from(
                unmanagedArray, 
                (UIntPtr)data.Length
            );
            
            return new NxData(handle, new RawNxData {
                data = unmanagedArray,
                size = (UIntPtr)data.Length
            });
        }

        public byte[] ToBytes()
        {
            if (_disposed) throw new ObjectDisposedException(nameof(NxData));
            if (_rawData.size == UIntPtr.Zero) return Array.Empty<byte>();
            
            byte[] bytes = new byte[Size];
            Marshal.Copy(_rawData.data, bytes, 0, Size);
            return bytes;
        }

        public void Dispose()
        {
            if (!_disposed)
            {
                RawNxDataNative.nexilis_nx_data_destroy(_handle);
                if (_rawData.data != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(_rawData.data);
                }
                _disposed = true;
                GC.SuppressFinalize(this);
            }
        }

        ~NxData() => Dispose();
    }
}