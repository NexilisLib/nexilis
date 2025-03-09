namespace Nexilis
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