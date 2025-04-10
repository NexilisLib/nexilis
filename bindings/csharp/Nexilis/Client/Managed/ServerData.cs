using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    /// <summary>
    /// Represents server data for the Nexilis client.
    /// </summary>
    public class ServerData : IDisposable
    {
        private IntPtr _serverDataPtr;

        /// <summary>
        /// Initializes a new instance of the <see cref="ServerData"/> class.
        /// </summary>
        public ServerData()
        {
            _serverDataPtr = ServerDataNative.nexilis_server_data_create();
        }

        /// <summary>
        /// Gets the native pointer to the server data.
        /// </summary>
        public IntPtr ServerDataPtr => _serverDataPtr;

        /// <summary>
        /// Releases the resources used by the <see cref="ServerData"/>.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the <see cref="ServerData"/> and optionally releases the managed resources.
        /// </summary>
        /// <param name="disposing">True to release both managed and unmanaged resources; false to release only unmanaged resources.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (_serverDataPtr != IntPtr.Zero)
            {
                ServerDataNative.nexilis_server_data_destroy(_serverDataPtr);
                _serverDataPtr = IntPtr.Zero;
            }
        }

        /// <summary>
        /// Finalizes an instance of the <see cref="ServerData"/> class.
        /// </summary>
        ~ServerData()
        {
            Dispose(false);
        }

        /// <summary>
        /// Sets the username for the server data.
        /// </summary>
        /// <param name="username">The username to set.</param>
        public void SetUsername(string username)
        {
            ServerDataNative.nexilis_server_data_set_username(_serverDataPtr, username);
        }

        /// <summary>
        /// Gets the username from the server data.
        /// </summary>
        /// <returns>The username from the server data.</returns>
        public string GetUsername()
        {
            return ServerDataNative.nexilis_server_data_get_username(_serverDataPtr);
        }

        /// <summary>
        /// Sets the password for the server data.
        /// </summary>
        /// <param name="password">The password to set.</param>
        public void SetPassword(string password)
        {
            ServerDataNative.nexilis_server_data_set_password(_serverDataPtr, password);
        }

        /// <summary>
        /// Gets the password from the server data.
        /// </summary>
        /// <returns>The password from the server data.</returns>
        public string GetPassword()
        {
            return ServerDataNative.nexilis_server_data_get_password(_serverDataPtr);
        }

        /// <summary>
        /// Sets the Boost TCP server address for the server data.
        /// </summary>
        /// <param name="address">The Boost TCP server address to set.</param>
        public void SetBoostTcp(string address)
        {
            ServerDataNative.nexilis_server_data_set_boost_tcp(_serverDataPtr, address);
        }

        /// <summary>
        /// Gets the Boost TCP server address from the server data.
        /// </summary>
        /// <returns>The Boost TCP server address from the server data.</returns>
        public string GetBoostTcpServerAddress()
        {
            return ServerDataNative.nexilis_server_data_get_boost_tcp_server_adress(_serverDataPtr);
        }

        /// <summary>
        /// Sets the Boost UDP server address for the server data.
        /// </summary>
        /// <param name="address">The Boost UDP server address to set.</param>
        public void SetBoostUdp(string address)
        {
            ServerDataNative.nexilis_server_data_set_boost_udp(_serverDataPtr, address);
        }

        /// <summary>
        /// Gets the Boost UDP server address from the server data.
        /// </summary>
        /// <returns>The Boost UDP server address from the server data.</returns>   
        public string GetBoostUdpServerAddress()
        {
            return ServerDataNative.nexilis_server_data_get_boost_udp_server_adress(_serverDataPtr);
        }
    }
}