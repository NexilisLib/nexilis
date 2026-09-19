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
