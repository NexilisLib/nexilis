using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    /// <summary>
    /// Represents client configuration for the Nexilis client.
    /// </summary>
    public class ClientConfig : IDisposable
    {
        private IntPtr _configPtr;

        /// <summary>
        /// Initializes a new instance of the <see cref="ClientConfig"/> class.
        /// </summary>
        public ClientConfig()
        {
            _configPtr = ClientConfigNative.nexilis_client_config_create();
        }

        /// <summary>
        /// Gets the native pointer to the config.
        /// </summary>
        public IntPtr ConfigPtr => _configPtr;

        /// <summary>
        /// Releases the resources used by the <see cref="ClientConfig"/>.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the <see cref="ClientConfig"/> and optionally releases the managed resources.
        /// </summary>
        /// <param name="disposing">True to release both managed and unmanaged resources; false to release only unmanaged resources.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (_configPtr != IntPtr.Zero)
            {
                ClientConfigNative.nexilis_client_config_destroy(_configPtr);
                _configPtr = IntPtr.Zero;
            }
        }

        /// <summary>
        /// Finalizes an instance of the <see cref="ClientConfig"/> class.
        /// </summary>
        ~ClientConfig()
        {
            Dispose(false);
        }

        /// <summary>
        /// Sets the authentication mode for the config.
        /// </summary>
        /// <param name="mode">The authentication mode (0=empty, 1=skip, 2=password_protected).</param>
        public void SetMode(int mode)
        {
            ClientConfigNative.nexilis_client_config_set_mode(_configPtr, mode);
        }

        /// <summary>
        /// Sets the password for the config.
        /// </summary>
        /// <param name="password">The password to set.</param>
        public void SetPassword(string password)
        {
            ClientConfigNative.nexilis_client_config_set_password(_configPtr, password);
        }

        /// <summary>
        /// Sets the password for the config.
        /// </summary>
        /// <param name="password">The password to set.</param>
        public ClientConfig Password(string password)
        {
            ClientConfigNative.nexilis_client_config_set_password(_configPtr, password);
            return this;
        }

        /// <summary>
        /// Gets the password from the config.
        /// </summary>
        /// <returns>The password from the config.</returns>
        public string GetPassword()
        {
            return ClientConfigNative.nexilis_client_config_get_password(_configPtr);
        }

        /// <summary>
        /// Sets the Boost TCP server address for the config.
        /// </summary>
        /// <param name="address">The Boost TCP server address to set.</param>
        public void SetBoostTCPAddress(string address)
        {
            ClientConfigNative.nexilis_client_config_set_boost_tcp_address(_configPtr, address);
        }

        /// <summary>
        /// Sets the Boost TCP server address for the config.
        /// </summary>
        /// <param name="address">The Boost TCP server address to set.</param>
        public ClientConfig BoostTCP(string address)
        {
            ClientConfigNative.nexilis_client_config_set_boost_tcp_address(_configPtr, address);
            return this;
        }

        /// <summary>
        /// Gets the Boost TCP server address from the config.
        /// </summary>
        /// <returns>The Boost TCP server address from the config.</returns>
        public string GetBoostTCPServerAddress()
        {
            return ClientConfigNative.nexilis_client_config_get_boost_tcp_server_address(_configPtr);
        }

        /// <summary>
        /// Sets the Boost UDP server address for the config.
        /// </summary>
        /// <param name="address">The Boost UDP server address to set.</param>
        public void SetBoostUDP(string address)
        {
            ClientConfigNative.nexilis_client_config_set_boost_udp_address(_configPtr, address);
        }

        /// <summary>
        /// Sets the Boost UDP server address for the config.
        /// </summary>
        /// <param name="address">The Boost UDP server address to set.</param>
        public ClientConfig BoostUDP(string address)
        {
            ClientConfigNative.nexilis_client_config_set_boost_udp_address(_configPtr, address);
            return this;
        }

        /// <summary>
        /// Gets the Boost UDP server address from the config.
        /// </summary>
        /// <returns>The Boost UDP server address from the config.</returns>
        public string GetBoostUdpServerAddress()
        {
            return ClientConfigNative.nexilis_client_config_get_boost_udp_server_address(_configPtr);
        }
    }
}
