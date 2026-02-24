using System;

namespace Nexilis.Logger
{
    /// <summary>
    /// Represents a file handler for logging messages to a file.
    /// </summary>
    public class FileHandler : IDisposable
    {
        private IntPtr _handlerPtr;

        /// <summary>
        /// Initializes a new instance of the <see cref="FileHandler"/> class.
        /// </summary>
        /// <exception cref="InvalidOperationException">Thrown if the native file handler cannot be created.</exception>
        public FileHandler(string filename)
        {
            _handlerPtr = FileHandlerNative.nexilis_logger_FileHandler_create(filename);
            if (_handlerPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create native console handler.");
            }
        }

        /// <summary>
        /// Gets the native pointer to the file handler.
        /// </summary>
        public IntPtr HandlerPtr => _handlerPtr;

        /// <summary>
        /// Releases the resources used by the <see cref="FileHandler"/>.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Releases the unmanaged resources used by the <see cref="FileHandler"/> and optionally releases the managed resources.
        /// </summary>
        /// <param name="disposing">True to release both managed and unmanaged resources; false to release only unmanaged resources.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (_handlerPtr != IntPtr.Zero)
            {
                FileHandlerNative.nexilis_logger_FileHandler_destroy(_handlerPtr);
                _handlerPtr = IntPtr.Zero;
            }
        }

        /// <summary>
        /// Finalizes an instance of the <see cref="FileHandler"/> class.
        /// </summary>
        ~FileHandler()
        {
            Dispose(false);
        }

        /// <summary>
        /// Gets the unique identifier of the file handler.
        /// </summary>
        /// <returns>The unique identifier of the file handler.</returns>
        /// <exception cref="InvalidOperationException">Thrown if the file handler pointer is null.</exception>
        public ulong GetId()
        {
            if (_handlerPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("The file handler pointer is invalid.");
            }
            return FileHandlerNative.nexilis_logger_FileHandler_get_id(_handlerPtr);
        }
    }
}

