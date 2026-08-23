using System;
using Xunit;
using Nexilis;

namespace Nexilis.Tests
{
    public class Vector3Tests
    {
        [Fact]
        public void CreateDefault()
        {
            using var v = new Vector3<float>(0.0f, 0.0f, 0.0f);

            Assert.Equal(0.0f, v.X);
            Assert.Equal(0.0f, v.Y);
            Assert.Equal(0.0f, v.Z);
        }

        [Fact]
        public void CreateWithValues()
        {
            using var v = new Vector3<float>(1.0f, 2.0f, 3.0f);

            Assert.Equal(1.0f, v.X);
            Assert.Equal(2.0f, v.Y);
            Assert.Equal(3.0f, v.Z);
        }

        [Fact]
        public void SetX()
        {
            using var v = new Vector3<float>(0.0f, 0.0f, 0.0f);
            v.X = 5.5f;

            Assert.Equal(5.5f, v.X);
            Assert.Equal(0.0f, v.Y);
            Assert.Equal(0.0f, v.Z);
        }

        [Fact]
        public void SetY()
        {
            using var v = new Vector3<float>(0.0f, 0.0f, 0.0f);
            v.Y = -3.0f;

            Assert.Equal(-3.0f, v.Y);
            Assert.Equal(0.0f, v.X);
            Assert.Equal(0.0f, v.Z);
        }

        [Fact]
        public void SetZ()
        {
            using var v = new Vector3<float>(0.0f, 0.0f, 0.0f);
            v.Z = 100.0f;

            Assert.Equal(100.0f, v.Z);
            Assert.Equal(0.0f, v.X);
            Assert.Equal(0.0f, v.Y);
        }

        [Fact]
        public void CopyConstructor()
        {
            using var original = new Vector3<float>(1.5f, -2.5f, 3.0f);
            using var copy = new Vector3<float>(original);

            Assert.Equal(original.X, copy.X);
            Assert.Equal(original.Y, copy.Y);
            Assert.Equal(original.Z, copy.Z);

            // Modifying the copy must not affect the original.
            copy.X = 42.0f;
            Assert.Equal(1.5f, original.X);
        }

        [Fact]
        public void SerializeDeserializeRoundtrip()
        {
            using var original = new Vector3<float>(1.5f, -2.5f, 3.0f);
            byte[] buffer = original.Serialize();

            Assert.Equal(12, buffer.Length);

            using var deserialized = Vector3<float>.Deserialize(buffer);
            Assert.Equal(1.5f, deserialized.X);
            Assert.Equal(-2.5f, deserialized.Y);
            Assert.Equal(3.0f, deserialized.Z);
        }

        [Fact]
        public void SerializeDeserializeZero()
        {
            using var original = new Vector3<float>(0.0f, 0.0f, 0.0f);
            byte[] buffer = original.Serialize();

            using var deserialized = Vector3<float>.Deserialize(buffer);
            Assert.Equal(0.0f, deserialized.X);
            Assert.Equal(0.0f, deserialized.Y);
            Assert.Equal(0.0f, deserialized.Z);
        }

        [Fact]
        public void DeserializeRejectsWrongSize()
        {
            Assert.Throws<ArgumentException>(() => Vector3<float>.Deserialize(new byte[8]));
        }

        [Fact]
        public void WrapperFromInvalidNativePointerThrows()
        {
            // A vector holding NaN is considered invalid by the native library.
            using var v = new Vector3<float>(0.0f, 0.0f, 0.0f);
            v.X = float.NaN;

            Assert.Throws<InvalidOperationException>(() => new Vector3<float>(v.getNative().vec));
        }
    }
}
