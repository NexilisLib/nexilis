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
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    /// <summary>
    /// Represents a Boost UDP client.
    /// </summary>
    public class BoostUDPClient : IDisposable
    {
        private IntPtr _boostUdpClientPtr;

        /// <summary>
        /// Initializes a new instance of the <see cref="BoostUDPClient"/> class.
        /// </summary>
        public BoostUDPClient(ProtocolManager protocolManager, ClientAPI clientApi)
        {
            if (protocolManager == null)
            {
                throw new ArgumentNullException(nameof(protocolManager));
            }

            if (clientApi == null)
            {
                throw new ArgumentNullException(nameof(clientApi));
            }

            _boostUdpClientPtr = BoostUDPClientNative.nexilis_boost_udp_client_create(
                    protocolManager.ProtocolManagerPtr, clientApi.ClientApiPtr);
        }

        /// <summary>
        /// Gets the native pointer to the Boost UDP client.
        /// </summary>
        public IntPtr BoostUdpClientPtr => _boostUdpClientPtr;

        /// <summary>
        /// Releases the resources used by the <see cref="BoostUDPClient"/>.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the <see cref="BoostUDPClient"/> and optionally releases the managed resources.
        /// </summary>
        /// <param name="disposing">True to release both managed and unmanaged resources; false to release only unmanaged resources.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (_boostUdpClientPtr != IntPtr.Zero)
            {
                BoostUDPClientNative.nexilis_boost_udp_client_destroy(_boostUdpClientPtr);
                _boostUdpClientPtr = IntPtr.Zero;
            }
        }

        /// <summary>
        /// Finalizes an instance of the <see cref="BoostUDPClient"/> class.
        /// </summary>
        ~BoostUDPClient()
        {
            Dispose(false);
        }

        /// <summary>
        /// Gets the protocol type of the Boost UDP client.
        /// </summary>
        /// <returns>The protocol type.</returns>
        public ProtocolType GetProtocolType()
        {
            return BoostUDPClientNative.nexilis_boost_udp_client_get_type(_boostUdpClientPtr);
        }

        /// <summary>
        /// Starts the Boost UDP client.
        /// </summary>
        public void Start()
        {
            BoostUDPClientNative.nexilis_boost_udp_client_start(_boostUdpClientPtr);
        }

        /// <summary>
        /// Stops the Boost UDP client.
        /// </summary>
        public void Stop()
        {
            BoostUDPClientNative.nexilis_boost_udp_client_stop(_boostUdpClientPtr);
        }

        /// <summary>
        /// Gets a value indicating whether the client is connected.
        /// </summary>
        /// <returns>True if the client is connected; otherwise, false.</returns>
        public bool IsConnected()
        {
            return BoostUDPClientNative.nexilis_boost_udp_client_is_connected(_boostUdpClientPtr);
        }

        /// <summary>
        /// Sends a message using the Boost UDP client.
        /// </summary>
        /// <param name="message">The message to send.</param>
        /// <param name="length">The length of the message.</param>
        public void SendMessage(byte[] message, uint length)
        {
            if (message == null || message.Length < length)
            {
                throw new ArgumentException("Invalid message or length.");
            }

            BoostUDPClientNative.nexilis_boost_udp_client_send_message(_boostUdpClientPtr, message, length);
        }

        /// <summary>
        /// Sends a message using the Boost UDP client with a callback.
        /// </summary>
        /// <param name="message">The message to send.</param>
        /// <param name="callback">The callback to invoke when the message is sent.</param>
        public void SendMessageWithCallback(string message, IntPtr callback)
        {
            if (string.IsNullOrEmpty(message))
            {
                throw new ArgumentException("Invalid message.");
            }

            byte[] messageBytes = System.Text.Encoding.UTF8.GetBytes(message);
            BoostUDPClientNative.nexilis_boost_udp_client_send_message_with_callback(
                    _boostUdpClientPtr, messageBytes, (uint)messageBytes.Length, callback);
        }
    }
}
