using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public class ClientAPI : IDisposable
    {
        private IntPtr _clientApiPtr;

        /// <summary>
        /// Initializes a new instance of the <see cref="ClientAPI"/> class.
        /// </summary>
        public ClientAPI(ServerData serverData)
        {
            _clientApiPtr = ClientAPINative.nexilis_client_api_create(serverData.ServerDataPtr);
        }

        /// <summary>
        /// Gets the native pointer to the client API.
        /// </summary>
        public IntPtr ClientApiPtr => _clientApiPtr;

        /// <summary>
        /// Get active rooms from the server.
        /// </summary>
        public RoomsCollection GetActiveRooms()
        {
            var ptr = ClientAPINative.nexilis_client_api_get_active_rooms(_clientApiPtr);
            return new RoomsCollection(ptr);
        }

        /// <summary>
        /// Get the unique id of "this" client.
        /// </summary>
        public ulong GetClientId()
        {
            return ClientAPINative.nexilis_client_api_get_client_id(_clientApiPtr);
        }

        /// <summary>
        /// Get the id of the room where this client currently is.
        /// </summary>
        public ulong ClientRoomId()
        {
            return ClientAPINative.nexilis_client_api_client_room_id(_clientApiPtr);
        }

        /// <summary>
        /// Get room object from the root id.
        /// </summary>
        public Room GetRoomFromId(ulong room_id)
        {
            var room_ptr = ClientAPINative.nexilis_client_api_get_room(_clientApiPtr, room_id);
            // Construct room from native pointer.
            return new Room(room_ptr);
        }

        /// <summary>
        /// Releases the resources used by the <see cref="ClientAPI"/>.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the <see cref="ClientAPI"/> and optionally releases the managed resources.
        /// </summary>
        /// <param name="disposing">True to release both managed and unmanaged resources; false to release only unmanaged resources.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (_clientApiPtr != IntPtr.Zero)
            {
                ClientAPINative.nexilis_client_api_destroy(_clientApiPtr);
                _clientApiPtr = IntPtr.Zero;
            }
        }

        /// <summary>
        /// Finalizes an instance of the <see cref="ClientAPI"/> class.
        /// </summary>
        ~ClientAPI()
        {
            Dispose(false);
        }
    }
}
