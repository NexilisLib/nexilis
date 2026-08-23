using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    /// <summary>
    /// A message sent to a room by a client session.
    /// </summary>
    public class Communication : IDisposable
    {
        IntPtr _nativePtr;
        bool _disposed = false;
        bool _ownsNativeInstance;

        /// <summary>
        /// Creates a new communication with a payload and a sender.
        /// </summary>
        public Communication(string payload, ClientSession sender)
        {
            if (string.IsNullOrEmpty(payload))
            {
                throw new ArgumentException("Payload must not be null or empty.", nameof(payload));
            }
            if (sender == null)
            {
                throw new ArgumentNullException(nameof(sender));
            }

            _nativePtr = RoomNative.nexilis_communication_create(payload, sender.GetNativePointer());
            if (_nativePtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create native communication.");
            }
            _ownsNativeInstance = true;
        }

        internal Communication(IntPtr nativePointer, bool ownsNativeInstance)
        {
            if (nativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(nativePointer));
            }
            _nativePtr = nativePointer;
            _ownsNativeInstance = ownsNativeInstance;
        }

        /// <summary>
        /// Gets the payload of the communication.
        /// </summary>
        public string GetPayload()
        {
            ThrowIfDisposed();

            IntPtr payloadPtr = RoomNative.nexilis_communication_get_payload(_nativePtr);
            if (payloadPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to get communication payload.");
            }

            try
            {
                return Marshal.PtrToStringAnsi(payloadPtr);
            }
            finally
            {
                // The native side allocates the string with malloc.
                Marshal.FreeHGlobal(payloadPtr);
            }
        }

        /// <summary>
        /// Gets the unique id of the communication. Zero until the
        /// communication has been added to a room.
        /// </summary>
        public ulong GetId()
        {
            ThrowIfDisposed();
            return RoomNative.nexilis_communication_get_id(_nativePtr);
        }

        /// <summary>
        /// Gets the native pointer to the sender's session wrapper.
        /// </summary>
        public IntPtr GetSender()
        {
            ThrowIfDisposed();
            return RoomNative.nexilis_communication_get_client(_nativePtr);
        }

        /// <summary>
        /// Gets the native pointer to the communication.
        /// </summary>
        public IntPtr NativePointer => _nativePtr;

        // The room takes ownership of the underlying message (it is moved
        // in natively), so disposal must not destroy it afterwards.
        internal void MarkOwnershipTransferred()
        {
            _ownsNativeInstance = false;
        }

        void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(Communication));
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_ownsNativeInstance && _nativePtr != IntPtr.Zero)
                {
                    RoomNative.nexilis_communication_destroy(_nativePtr);
                    _nativePtr = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        ~Communication()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}
