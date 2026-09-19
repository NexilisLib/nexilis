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
using Nexilis;

namespace Nexilis.Tests
{
    /// <summary>
    /// Type-independent Vector3 tests. Concrete vector types derive from this
    /// class and only provide the component values used by the shared tests.
    /// </summary>
    public abstract class Vector3Tests<T>
    {
        protected readonly T X;
        protected readonly T Y;
        protected readonly T Z;
        protected readonly T NewX;
        protected readonly T NewY;
        protected readonly T NewZ;

        protected Vector3Tests(T x, T y, T z, T newX, T newY, T newZ)
        {
            TestEnvironment.Initialize();
            X = x;
            Y = y;
            Z = z;
            NewX = newX;
            NewY = newY;
            NewZ = newZ;
        }

        [Fact]
        public void CreateDefault()
        {
            using var v = new Vector3<T>(default(T)!, default(T)!, default(T)!);

            Assert.Equal(default(T)!, v.X);
            Assert.Equal(default(T)!, v.Y);
            Assert.Equal(default(T)!, v.Z);
        }

        [Fact]
        public void CreateWithValues()
        {
            using var v = new Vector3<T>(X, Y, Z);

            Assert.Equal(X, v.X);
            Assert.Equal(Y, v.Y);
            Assert.Equal(Z, v.Z);
        }

        [Fact]
        public void SetX()
        {
            using var v = new Vector3<T>(default(T)!, default(T)!, default(T)!);
            v.X = NewX;

            Assert.Equal(NewX, v.X);
            Assert.Equal(default(T)!, v.Y);
            Assert.Equal(default(T)!, v.Z);
        }

        [Fact]
        public void SetY()
        {
            using var v = new Vector3<T>(default(T)!, default(T)!, default(T)!);
            v.Y = NewY;

            Assert.Equal(NewY, v.Y);
            Assert.Equal(default(T)!, v.X);
            Assert.Equal(default(T)!, v.Z);
        }

        [Fact]
        public void SetZ()
        {
            using var v = new Vector3<T>(default(T)!, default(T)!, default(T)!);
            v.Z = NewZ;

            Assert.Equal(NewZ, v.Z);
            Assert.Equal(default(T)!, v.X);
            Assert.Equal(default(T)!, v.Y);
        }

        [Fact]
        public void CopyConstructor()
        {
            using var original = new Vector3<T>(X, Y, Z);
            using var copy = new Vector3<T>(original);

            Assert.Equal(original.X, copy.X);
            Assert.Equal(original.Y, copy.Y);
            Assert.Equal(original.Z, copy.Z);

            // Modifying the copy must not affect the original.
            copy.X = NewX;
            Assert.Equal(X, original.X);
        }

        [Fact]
        public void SerializeDeserializeRoundtrip()
        {
            using var original = new Vector3<T>(X, Y, Z);
            byte[] buffer = original.Serialize();

            Assert.Equal(12, buffer.Length);

            using var deserialized = Vector3<T>.Deserialize(buffer);
            Assert.Equal(X, deserialized.X);
            Assert.Equal(Y, deserialized.Y);
            Assert.Equal(Z, deserialized.Z);
        }

        [Fact]
        public void SerializeDeserializeZero()
        {
            using var original = new Vector3<T>(default(T)!, default(T)!, default(T)!);
            byte[] buffer = original.Serialize();

            using var deserialized = Vector3<T>.Deserialize(buffer);
            Assert.Equal(default(T)!, deserialized.X);
            Assert.Equal(default(T)!, deserialized.Y);
            Assert.Equal(default(T)!, deserialized.Z);
        }

        [Fact]
        public void DeserializeRejectsWrongSize()
        {
            Assert.Throws<ArgumentException>(() => Vector3<T>.Deserialize(new byte[8]));
        }
    }

    public class Vector3FloatTests : Vector3Tests<float>
    {
        public Vector3FloatTests() : base(1.5f, -2.5f, 3.0f, 5.5f, -3.0f, 100.0f) { }

        [Fact]
        public void WrapperFromInvalidNativePointerThrows()
        {
            // A vector holding NaN is considered invalid by the native library.
            using var v = new Vector3<float>(0.0f, 0.0f, 0.0f);
            v.X = float.NaN;

            Assert.Throws<InvalidOperationException>(() => new Vector3<float>(v.getNative().vec));
        }
    }

    public class Vector3IntTests : Vector3Tests<int>
    {
        public Vector3IntTests() : base(1, -2, 3, 42, -7, 1000) { }

        [Fact]
        public void SerializeDeserializeIntExtremes()
        {
            using var original = new Vector3<int>(int.MaxValue, 0, int.MinValue);
            byte[] buffer = original.Serialize();

            using var deserialized = Vector3<int>.Deserialize(buffer);
            Assert.Equal(int.MaxValue, deserialized.X);
            Assert.Equal(0, deserialized.Y);
            Assert.Equal(int.MinValue, deserialized.Z);
        }
    }

    public class Vector3UIntTests : Vector3Tests<ulong>
    {
        public Vector3UIntTests() : base(1UL, 2UL, 3UL, 42UL, 7UL, 1000UL) { }

        [Fact]
        public void SerializeDeserializeUInt32Extremes()
        {
            // Serialization stores components as 32-bit, so uint32 extremes round-trip.
            using var original = new Vector3<ulong>(uint.MaxValue, 0UL, uint.MaxValue);
            byte[] buffer = original.Serialize();

            using var deserialized = Vector3<ulong>.Deserialize(buffer);
            Assert.Equal(uint.MaxValue, deserialized.X);
            Assert.Equal(0UL, deserialized.Y);
            Assert.Equal(uint.MaxValue, deserialized.Z);
        }
    }
}
