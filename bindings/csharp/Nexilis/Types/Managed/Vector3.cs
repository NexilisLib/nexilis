using System;
using Nexilis.Logger;

namespace Nexilis
{

public class Vector3<T> : IDisposable
{
    static NxLogger _logger = new NxLogger("Vector3");
    public static void InitializeLogger(Action<Logger.LogLevel, string> logCallback) => _logger.Setup(logCallback);

    interface IVector3Operations
    {
        RawVector3 Create(T x, T y, T z);
        RawVector3 CreateDefault();
        void Destroy(RawVector3 vector);
        void GetComponents(RawVector3 vector, out T x, out T y, out T z);
        void SetComponents(RawVector3 vector, T x, T y, T z);
        byte[] Serialize(RawVector3 vector);
        RawVector3 Deserialize(byte[] data);
    }

    class FloatOperations : IVector3Operations
    {
        public RawVector3 Create(T x, T y, T z)
        {
            return Vector3Native.nexilis_vector3f_create((float)(object)x, (float)(object)y, (float)(object)z);
        }

        public RawVector3 CreateDefault() => Vector3Native.nexilis_vector3f_create_default();

        public void Destroy(RawVector3 vector) => Vector3Native.nexilis_vector3f_destroy(vector);

        public void GetComponents(RawVector3 vector, out T x, out T y, out T z)
        {
            float _x = Vector3Native.nexilis_vector3f_get_x(vector);
            float _y = Vector3Native.nexilis_vector3f_get_y(vector);
            float _z = Vector3Native.nexilis_vector3f_get_z(vector);

            x = (T)(object)_x;
            y = (T)(object)_y;
            z = (T)(object)_z;
        }

        public void SetComponents(RawVector3 vector, T? x, T? y, T? z)
        {
            Vector3Native.nexilis_vector3f_set_x(vector, (float)(object)x!);
            Vector3Native.nexilis_vector3f_set_y(vector, (float)(object)y!);
            Vector3Native.nexilis_vector3f_set_z(vector, (float)(object)z!);
        }

        public byte[] Serialize(RawVector3 vector)
        {
            byte[] data = new byte[12]; // 3 floats
            Vector3Native.nexilis_vector3f_serialize(vector, data);
            return data;
        }

        public RawVector3 Deserialize(byte[] data) => Vector3Native.nexilis_vector3f_deserialize(data);
    }

    /*
    // Int implementation
    class IntOperations : IVector3Operations
    {
        // Similar implementation as FloatOperations but for int
        // ...
    }

    // UInt64 implementation
    class ULongOperations : IVector3Operations
    {
        // Similar implementation as FloatOperations but for ulong
        // ...
    }
    */

    static readonly IVector3Operations _operations;
    RawVector3 _nativePtr;
    bool _disposed = false;

    static Vector3()
    {
        Type type = typeof(T);
        if (type == typeof(float)) _operations = new FloatOperations();
        //else if (type == typeof(int)) _operations = new IntOperations();
        //else if (type == typeof(ulong)) _operations = new ULongOperations();
        else throw new NotSupportedException($"Type {type.Name} is not supported by Vector3");
    }

    public Vector3(T x, T y, T z)
    {
        _nativePtr = _operations.Create(x, y, z);
        if (_nativePtr.vector == IntPtr.Zero)
            throw new Exception("Failed to create native Vector3");
    }

    public RawVector3 getNative()
    {
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
        var nativePtr = _operations.Deserialize(data);
        if (nativePtr.vector == IntPtr.Zero)
            throw new Exception("Failed to deserialize Vector3<T>");

        return new Vector3<T>(nativePtr.vector);
    }

    Vector3(IntPtr nativePtr)
    {
        _nativePtr.vector = nativePtr;
    }

    ~Vector3()
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
            if (_nativePtr.vector != IntPtr.Zero)
            {
                _operations.Destroy(_nativePtr);
                _nativePtr.vector = IntPtr.Zero;
            }
            _disposed = true;
        }
    }

    public override string ToString() => $"Vector3<{typeof(T).Name}>({X}, {Y}, {Z})";
}

}
