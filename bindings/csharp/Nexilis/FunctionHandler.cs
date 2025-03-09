using System;
using System.Runtime.InteropServices;

namespace Nexilis
{
    public class FunctionHandler : IDisposable
    {
        // Native function handler pointer
        private IntPtr _handlerPtr;

        // Delegate for the callback function
        private readonly Action<LogLevel, string> _function;

        // Constructor
        public FunctionHandler(Action<LogLevel, string> function)
        {
            if (function == null)
                throw new ArgumentNullException(nameof(function));

            _function = function;

            // Create the native function handler
            _handlerPtr = FunctionHandlerNative.nexilis_logger_FunctionHandler_create(OnLogMessage);
            if (_handlerPtr == IntPtr.Zero)
                throw new InvalidOperationException("Failed to create native function handler.");
        }

        /// <summary>
        /// Gets the native pointer to the file handler.
        /// </summary>
        public IntPtr HandlerPtr => _handlerPtr;

        // Callback function for native code
        private void OnLogMessage(LogLevel logLevel, IntPtr messagePtr)
        {
            string? message = Marshal.PtrToStringAnsi(messagePtr);
            if (message == null)
            {
                Console.WriteLine("Warning: Received null message from native code.");
                return;
            }
            _function(logLevel, message);
        }

        // Get the handler ID
        public ulong GetId()
        {
            if (_handlerPtr == IntPtr.Zero)
                throw new ObjectDisposedException(nameof(FunctionHandler));

            return FunctionHandlerNative.nexilis_logger_FunctionHandler_get_id(_handlerPtr);
        }

        // Dispose pattern
        public void Dispose()
        {
            if (_handlerPtr != IntPtr.Zero)
            {
                FunctionHandlerNative.nexilis_logger_FunctionHandler_destroy(_handlerPtr);
                _handlerPtr = IntPtr.Zero;
            }
            GC.SuppressFinalize(this);
        }

        ~FunctionHandler()
        {
            Dispose();
        }
    }
}