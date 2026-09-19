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

namespace Nexilis.Logger
{
    /// <summary>
    /// Represents a console handler for logging messages to the console.
    /// </summary>
    public class ConsoleHandler : IDisposable
    {
        private IntPtr _handlerPtr;

        /// <summary>
        /// Initializes a new instance of the <see cref="ConsoleHandler"/> class.
        /// </summary>
        /// <exception cref="InvalidOperationException">Thrown if the native console handler cannot be created.</exception>
        public ConsoleHandler()
        {
            _handlerPtr = ConsoleHandlerNative.nexilis_logger_ConsoleHandler_create();
            if (_handlerPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create native console handler.");
            }
        }

        /// <summary>
        /// Gets the native pointer to the console handler.
        /// </summary>
        public IntPtr HandlerPtr => _handlerPtr;

        /// <summary>
        /// Releases the resources used by the <see cref="ConsoleHandler"/>.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the <see cref="ConsoleHandler"/> and optionally releases the managed resources.
        /// </summary>
        /// <param name="disposing">True to release both managed and unmanaged resources; false to release only unmanaged resources.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (_handlerPtr != IntPtr.Zero)
            {
                ConsoleHandlerNative.nexilis_logger_ConsoleHandler_destroy(_handlerPtr);
                _handlerPtr = IntPtr.Zero;
            }
        }

        /// <summary>
        /// Finalizes an instance of the <see cref="ConsoleHandler"/> class.
        /// </summary>
        ~ConsoleHandler()
        {
            Dispose(false);
        }

        /// <summary>
        /// Gets the unique identifier of the console handler.
        /// </summary>
        /// <returns>The unique identifier of the console handler.</returns>
        /// <exception cref="InvalidOperationException">Thrown if the native console handler is invalid.</exception>
        public ulong GetId()
        {
            if (_handlerPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("Invalid native console handler.");
            }
            return ConsoleHandlerNative.nexilis_logger_ConsoleHandler_get_id(_handlerPtr);
        }
    }
}

