using System;
using System.Runtime.InteropServices;
using Xunit;
using Nexilis;

namespace Nexilis.Tests
{
    public class NxDataTests
    {
        public NxDataTests()
        {
            TestEnvironment.Initialize();
        }

        [Fact]
        public void Create()
        {
            var raw = RawNxDataNative.nexilis_nx_data_create(10);
            Assert.NotEqual(IntPtr.Zero, raw.data);
            Assert.Equal(10ul, RawNxDataNative.nexilis_nx_data_get_size(ref raw));

            RawNxDataNative.nexilis_nx_data_destroy(ref raw);
            Assert.Equal(IntPtr.Zero, raw.data);
        }

        [Fact]
        public void CreateFrom()
        {
            byte[] source = { 0x01, 0x02, 0x03, 0x04 };
            IntPtr sourcePtr = Marshal.AllocHGlobal(source.Length);
            try
            {
                Marshal.Copy(source, 0, sourcePtr, source.Length);

                var raw = RawNxDataNative.nexilis_nx_data_create_from(sourcePtr, 4);
                Assert.NotEqual(IntPtr.Zero, raw.data);
                Assert.Equal(4ul, RawNxDataNative.nexilis_nx_data_get_size(ref raw));

                using var data = NxData.Create(raw);
                Assert.Equal(source, data.ToBytes());
            }
            finally
            {
                Marshal.FreeHGlobal(sourcePtr);
            }
        }

        [Fact]
        public void CreateZeroSize()
        {
            var raw = RawNxDataNative.nexilis_nx_data_create(0);
            Assert.NotEqual(IntPtr.Zero, raw.data);
            Assert.Equal(0ul, RawNxDataNative.nexilis_nx_data_get_size(ref raw));

            // Managed wrapper rejects zero-size data.
            Assert.Throws<ArgumentException>(() => NxData.Create(raw));

            RawNxDataNative.nexilis_nx_data_destroy(ref raw);
        }

        [Fact]
        public void ManagedWrapperSizeAndBytes()
        {
            var raw = RawNxDataNative.nexilis_nx_data_create(16);
            using var data = NxData.Create(raw);

            Assert.Equal(16ul, data.Size);
            Assert.Equal(16, data.ToBytes().Length);
            Assert.NotEqual(IntPtr.Zero, data.DataPointer);
        }

        [Fact]
        public void DisposeIsIdempotent()
        {
            var raw = RawNxDataNative.nexilis_nx_data_create(8);
            var data = NxData.Create(raw);

            data.Dispose();
            data.Dispose();

            Assert.Throws<ObjectDisposedException>(() => data.Size);
        }
    }
}
