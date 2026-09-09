using System;
using Nexilis.Logger;
using System.Runtime.InteropServices;

namespace Nexilis
{

public class Vector3<T> : IDisposable
{
    static NxLogger _logger = new NxLogger("Vector3");
    public static void InitializeLogger(Action<Logger.LogLevel, string> logCallback) => _logger.Setup(logCallback);

    static readonly IVector3Operations _operations;
    RawVector3 _nativePtr;
    bool _disposed = false;
    bool _ownsNativePointer;

    interface IVector3Operations
    {
        RawVector3 Create(T x, T y, T z);
        RawVector3 CreateDefault();
        void Destroy(ref RawVector3 vector);
        void GetComponents(RawVector3 vector, out T x, out T y, out T z);
        void SetComponents(RawVector3 vector, T x, T y, T z);
        byte[] Serialize(RawVector3 vector);
        RawVector3 Deserialize(byte[] data);
        bool IsValid(IntPtr nativePtr);
    }

    class FloatOperations : IVector3Operations
    {
        public RawVector3 Create(T x, T y, T z)
        {
            if (x is null) throw new ArgumentNullException(nameof(x));
            if (y is null) throw new ArgumentNullException(nameof(y));
            if (z is null) throw new ArgumentNullException(nameof(z));

            IntPtr wrapper = Vector3Native.nexilis_vector3f_create((float)(object)x, (float)(object)y, (float)(object)z);
            if (wrapper == IntPtr.Zero)
            {
                throw new Exception("Native vector creation failed");
            }
            return new RawVector3 { vec = wrapper };
        }

        public RawVector3 CreateDefault()
        {
            IntPtr wrapper = Vector3Native.nexilis_vector3f_create_default();
            return new RawVector3 { vec = wrapper };
        }

        public void Destroy(ref RawVector3 vector)
        {
            if (vector.IsValid)
            {
                Vector3Native.nexilis_vector3f_destroy(vector.vec);
                vector.vec = IntPtr.Zero;
            }
        }

        public void GetComponents(RawVector3 vector, out T x, out T y, out T z)
        {
            if (vector.vec == IntPtr.Zero)
            {
                throw new NullReferenceException("Native vector pointer is null");
            }

            try
            {
                float _x = Vector3Native.nexilis_vector3f_get_x(vector.vec);
                float _y = Vector3Native.nexilis_vector3f_get_y(vector.vec);
                float _z = Vector3Native.nexilis_vector3f_get_z(vector.vec);

                x = (T)(object)_x;
                y = (T)(object)_y;
                z = (T)(object)_z;
            }
            catch (Exception ex)
            {
                _logger.Error($"Failed to get components: {ex}");
                throw;
            }
        }

        public void SetComponents(RawVector3 vector, T? x, T? y, T? z)
        {
            Vector3Native.nexilis_vector3f_set_x(vector.vec, (float)(object)x!);
            Vector3Native.nexilis_vector3f_set_y(vector.vec, (float)(object)y!);
            Vector3Native.nexilis_vector3f_set_z(vector.vec, (float)(object)z!);
        }

        public byte[] Serialize(RawVector3 vector)
        {
            byte[] data = new byte[12]; // 3 floats
            Vector3Native.nexilis_vector3f_serialize(vector.vec, data);
            return data;
        }

        public RawVector3 Deserialize(byte[] data)
        {
            IntPtr wrapper = Vector3Native.nexilis_vector3f_deserialize(data);
            return new RawVector3 { vec = wrapper };
        }

        public bool IsValid(IntPtr nativePtr)
        {
            return Vector3Native.nexilis_vector3f_is_valid(nativePtr);
        }
    }

    // Int implementation
    class IntOperations : IVector3Operations
    {
        public RawVector3 Create(T x, T y, T z)
        {
            if (x is null) throw new ArgumentNullException(nameof(x));
            if (y is null) throw new ArgumentNullException(nameof(y));
            if (z is null) throw new ArgumentNullException(nameof(z));

            IntPtr wrapper = Vector3Native.nexilis_vector3i_create((int)(object)x, (int)(object)y, (int)(object)z);
            if (wrapper == IntPtr.Zero)
            {
                throw new Exception("Native vector creation failed");
            }
            return new RawVector3 { vec = wrapper };
        }

        public RawVector3 CreateDefault()
        {
            IntPtr wrapper = Vector3Native.nexilis_vector3i_create_default();
            return new RawVector3 { vec = wrapper };
        }

        public void Destroy(ref RawVector3 vector)
        {
            if (vector.IsValid)
            {
                Vector3Native.nexilis_vector3i_destroy(vector.vec);
                vector.vec = IntPtr.Zero;
            }
        }

        public void GetComponents(RawVector3 vector, out T x, out T y, out T z)
        {
            if (vector.vec == IntPtr.Zero)
            {
                throw new NullReferenceException("Native vector pointer is null");
            }

            try
            {
                int _x = Vector3Native.nexilis_vector3i_get_x(vector.vec);
                int _y = Vector3Native.nexilis_vector3i_get_y(vector.vec);
                int _z = Vector3Native.nexilis_vector3i_get_z(vector.vec);

                x = (T)(object)_x;
                y = (T)(object)_y;
                z = (T)(object)_z;
            }
            catch (Exception ex)
            {
                _logger.Error($"Failed to get components: {ex}");
                throw;
            }
        }

        public void SetComponents(RawVector3 vector, T? x, T? y, T? z)
        {
            Vector3Native.nexilis_vector3i_set_x(vector.vec, (int)(object)x!);
            Vector3Native.nexilis_vector3i_set_y(vector.vec, (int)(object)y!);
            Vector3Native.nexilis_vector3i_set_z(vector.vec, (int)(object)z!);
        }

        public byte[] Serialize(RawVector3 vector)
        {
            byte[] data = new byte[12]; // 3 int32
            Vector3Native.nexilis_vector3i_serialize(vector.vec, data);
            return data;
        }

        public RawVector3 Deserialize(byte[] data)
        {
            IntPtr wrapper = Vector3Native.nexilis_vector3i_deserialize(data);
            return new RawVector3 { vec = wrapper };
        }

        public bool IsValid(IntPtr nativePtr)
        {
            return Vector3Native.nexilis_vector3i_is_valid(nativePtr);
        }
    }

    // UInt64 implementation
    class ULongOperations : IVector3Operations
    {
        public RawVector3 Create(T x, T y, T z)
        {
            if (x is null) throw new ArgumentNullException(nameof(x));
            if (y is null) throw new ArgumentNullException(nameof(y));
            if (z is null) throw new ArgumentNullException(nameof(z));

            IntPtr wrapper = Vector3Native.nexilis_vector3u_create((ulong)(object)x, (ulong)(object)y, (ulong)(object)z);
            if (wrapper == IntPtr.Zero)
            {
                throw new Exception("Native vector creation failed");
            }
            return new RawVector3 { vec = wrapper };
        }

        public RawVector3 CreateDefault()
        {
            IntPtr wrapper = Vector3Native.nexilis_vector3u_create_default();
            return new RawVector3 { vec = wrapper };
        }

        public void Destroy(ref RawVector3 vector)
        {
            if (vector.IsValid)
            {
                Vector3Native.nexilis_vector3u_destroy(vector.vec);
                vector.vec = IntPtr.Zero;
            }
        }

        public void GetComponents(RawVector3 vector, out T x, out T y, out T z)
        {
            if (vector.vec == IntPtr.Zero)
            {
                throw new NullReferenceException("Native vector pointer is null");
            }

            try
            {
                ulong _x = Vector3Native.nexilis_vector3u_get_x(vector.vec);
                ulong _y = Vector3Native.nexilis_vector3u_get_y(vector.vec);
                ulong _z = Vector3Native.nexilis_vector3u_get_z(vector.vec);

                x = (T)(object)_x;
                y = (T)(object)_y;
                z = (T)(object)_z;
            }
            catch (Exception ex)
            {
                _logger.Error($"Failed to get components: {ex}");
                throw;
            }
        }

        public void SetComponents(RawVector3 vector, T? x, T? y, T? z)
        {
            Vector3Native.nexilis_vector3u_set_x(vector.vec, (ulong)(object)x!);
            Vector3Native.nexilis_vector3u_set_y(vector.vec, (ulong)(object)y!);
            Vector3Native.nexilis_vector3u_set_z(vector.vec, (ulong)(object)z!);
        }

        public byte[] Serialize(RawVector3 vector)
        {
            byte[] data = new byte[12]; // 3 uint32
            Vector3Native.nexilis_vector3u_serialize(vector.vec, data);
            return data;
        }

        public RawVector3 Deserialize(byte[] data)
        {
            IntPtr wrapper = Vector3Native.nexilis_vector3u_deserialize(data);
            return new RawVector3 { vec = wrapper };
        }

        public bool IsValid(IntPtr nativePtr)
        {
            return Vector3Native.nexilis_vector3u_is_valid(nativePtr);
        }
    }

    static Vector3()
    {
        Type type = typeof(T);
        if (type == typeof(float)) _operations = new FloatOperations();
        else if (type == typeof(int)) _operations = new IntOperations();
        else if (type == typeof(ulong)) _operations = new ULongOperations();
        else throw new NotSupportedException($"Type {type.Name} is not supported by Vector3");
    }

    public Vector3(T x, T y, T z)
    {
        try
        {
            _nativePtr = _operations.Create(x, y, z);
            if (_nativePtr.vec == IntPtr.Zero)
                throw new Exception("Failed to create native Vector3");

        }
        catch (Exception ex)
        {
            _logger.Error($"Failed to create Vector3: {ex}");
            throw;
        }
    }

    public Vector3(IntPtr nativePtr, bool ownsNativePointer = true)
    {
        if (nativePtr == IntPtr.Zero)
        {
            throw new ArgumentNullException(nameof(nativePtr));
        }

        if (!_operations.IsValid(nativePtr))
        {
            throw new InvalidOperationException("Invalid native pointer");
        }
        _nativePtr.vec = nativePtr;
        _ownsNativePointer = ownsNativePointer;
        _disposed = false;
    }

    public Vector3(Vector3<T> other)
    {
        if (other == null) throw new ArgumentNullException(nameof(other));
        if (other._disposed) throw new ObjectDisposedException("Source Vector3 is disposed");

        var x = other.X;
        var y = other.Y;
        var z = other.Z;
        _nativePtr = _operations.Create(x, y, z);
        _disposed = false;
    }

    public RawVector3 getNative()
    {
        if (_disposed) throw new ObjectDisposedException("Vector3<T>");
        if (_nativePtr.vec == IntPtr.Zero) throw new InvalidOperationException("Native pointer is null");

        _logger.Debug($"Getting native pointer: 0x{_nativePtr.vec.ToInt64():X}");
        return _nativePtr;
    }

    public T X
    {
        get
        {
            if (_disposed) throw new ObjectDisposedException("Vector3<T>");
            _operations.GetComponents(_nativePtr, out T x, out _, out _);
            return x;
        }
        set
        {
            if (_disposed) throw new ObjectDisposedException("Vector3<T>");
            _operations.GetComponents(_nativePtr, out _, out T y, out T z);
            _operations.SetComponents(_nativePtr, value, y, z);
        }
    }

    public T Y
    {
        get
        {
            if (_disposed) throw new ObjectDisposedException("Vector3<T>");
            _operations.GetComponents(_nativePtr, out _, out T y, out _);
            return y;
        }
        set
        {
            if (_disposed) throw new ObjectDisposedException("Vector3<T>");
            _operations.GetComponents(_nativePtr, out T x, out _, out T z);
            _operations.SetComponents(_nativePtr, x, value, z);
        }
    }

    public T Z
    {
        get
        {
            if (_disposed) throw new ObjectDisposedException("Vector3<T>");
            _operations.GetComponents(_nativePtr, out _, out _, out T z);
            return z;
        }
        set
        {
            if (_disposed) throw new ObjectDisposedException("Vector3<T>");
            _operations.GetComponents(_nativePtr, out T x, out T y, out _);
            _operations.SetComponents(_nativePtr, x, y, value);
        }
    }

    public byte[] Serialize()
    {
        if (_disposed) throw new ObjectDisposedException("Vector3<T>");
        return _operations.Serialize(_nativePtr);
    }

    public static Vector3<T> Deserialize(byte[] data)
    {
        if (data == null) throw new ArgumentNullException(nameof(data));
        if (data.Length != 12) throw new ArgumentException("Data must be 12 bytes for Vector3<float>");

        var nativePtr = _operations.Deserialize(data);
        if (nativePtr.vec == IntPtr.Zero)
            throw new Exception("Failed to deserialize Vector3<T>");

        return new Vector3<T>(nativePtr.vec);
    }

    ~Vector3()
    {
        _logger.Debug("Disposing Vector3<T>");
        Dispose(false);
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    public bool IsDisposed => _disposed;

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            try
            {
                if (_ownsNativePointer && _nativePtr.IsValid)
                {
                    _logger.Debug($"Disposing Vector3:{_nativePtr.vec} (disposing={disposing})");
                    _operations.Destroy(ref _nativePtr);
                }
            }
            catch (Exception ex)
            {
                _logger.Error($"Error during disposal: {ex}");
            }
            finally
            {
                _nativePtr.vec = IntPtr.Zero;
                _disposed = true;
            }
        }
    }

    public override string ToString() => $"Vector3<{typeof(T).Name}>({X}, {Y}, {Z})";
}

}
