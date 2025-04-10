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