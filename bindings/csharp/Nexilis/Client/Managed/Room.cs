using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public class Room : IDisposable
    {
        IntPtr _nativePtr;
        bool _disposed = false;

        public Room(RoomData roomData)
        {
            if (roomData.NativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(roomData));
            }
            _nativePtr = RoomNative.nexilis_room_create(roomData.NativePointer, IntPtr.Zero);
        }

        public Room(IntPtr nativePointer)
        {
            if (nativePointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(nativePointer));
            }
            _nativePtr = nativePointer;
        }

        public IReadOnlyList<ClientSession> GetClients()
        {
            ThrowIfDisposed();

            ulong numClients;
            IntPtr clientsArrayPtr = RoomNative.nexilis_room_get_clients(_nativePtr, out numClients);

            if (clientsArrayPtr == IntPtr.Zero || numClients == 0)
            {
                return Array.Empty<ClientSession>();
            }

            var clientSessions = new List<ClientSession>();
            IntPtr[] clientPointers = new IntPtr[numClients];

            try
            {
                Marshal.Copy(clientsArrayPtr, clientPointers, 0, (int)numClients);

                // Create managed wrappers.
                foreach (var ptr in clientPointers)
                {
                    if (ptr != IntPtr.Zero)
                    {
                        clientSessions.Add(new ClientSession(ptr, ownsNativeInstance: false));
                    }
                }
                return clientSessions.AsReadOnly();
            }
            finally
            {
                // Pass 0 to only free the outer array, not the individual wrappers.
                // The wrappers are owned by the returned ClientSession objects.
                // TODO: add nexilis_client_session_free_wrapper to properly free wrappers on ClientSession disposal.
                RoomNative.nexilis_room_free_client_array(clientsArrayPtr, 0);
            }

        }

        public ulong GetClientAmount()
        {
            return RoomNative.nexilis_room_get_client_amount(_nativePtr);
        }

        public ulong GetId()
        {
            return RoomNative.nexilis_room_get_id(_nativePtr);
        }

        public void AddClient(ClientSession client)
        {
            ThrowIfDisposed();
            if (client == null)
                throw new ArgumentNullException(nameof(client));

            RoomNative.nexilis_room_add_client(_nativePtr, client.GetNativePointer());
        }

        public void RemoveClient(ulong clientId)
        {
            ThrowIfDisposed();
            RoomNative.nexilis_room_remove_client(_nativePtr, clientId);
        }

        /// <summary>
        /// Adds a message to the room. The room takes ownership of the
        /// communication; it must not be disposed by the caller afterwards.
        /// </summary>
        public void AddMessage(Communication message)
        {
            ThrowIfDisposed();
            if (message == null)
            {
                throw new ArgumentNullException(nameof(message));
            }

            RoomNative.nexilis_room_add_message(_nativePtr, message.NativePointer);
            message.MarkOwnershipTransferred();
        }

        /// <summary>
        /// Gets copies of all messages in the room.
        /// </summary>
        public IReadOnlyList<Communication> GetMessages()
        {
            ThrowIfDisposed();

            ulong numMessages;
            IntPtr messagesArrayPtr = RoomNative.nexilis_room_get_messages(_nativePtr, out numMessages);

            if (messagesArrayPtr == IntPtr.Zero || numMessages == 0)
            {
                return Array.Empty<Communication>();
            }

            var communications = new List<Communication>();
            IntPtr[] messagePointers = new IntPtr[numMessages];

            try
            {
                Marshal.Copy(messagesArrayPtr, messagePointers, 0, (int)numMessages);

                foreach (var ptr in messagePointers)
                {
                    if (ptr != IntPtr.Zero)
                    {
                        communications.Add(new Communication(ptr, ownsNativeInstance: true));
                    }
                }
                return communications.AsReadOnly();
            }
            finally
            {
                // Free only the outer array; each Communication wrapper owns
                // its copied native instance and releases it on Dispose.
                RoomNative.nexilis_room_free_message_array(messagesArrayPtr, 0);
            }
        }

        /// <summary>
        /// Checks whether the room contains the given communication.
        /// </summary>
        public bool ContainsCommunication(Communication message)
        {
            ThrowIfDisposed();
            if (message == null)
            {
                throw new ArgumentNullException(nameof(message));
            }
            return RoomNative.nexilis_room_contains_communication(_nativePtr, message.NativePointer);
        }

        /// <summary>
        /// Checks whether the room contains a communication with the given id.
        /// </summary>
        public bool ContainsCommunication(ulong messageId)
        {
            ThrowIfDisposed();
            return RoomNative.nexilis_room_contains_communication_by_id(_nativePtr, messageId);
        }

        void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(Room));
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_nativePtr != IntPtr.Zero)
                {
                    RoomNative.nexilis_room_destroy(_nativePtr);
                    _nativePtr = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        ~Room()
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
