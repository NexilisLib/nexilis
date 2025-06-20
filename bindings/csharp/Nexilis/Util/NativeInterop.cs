using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace Nexilis
{
    public static class NativeInterop
    {
        private static readonly object _syncRoot = new object();
        private static int? _mainThreadId;

        /// <summary>
        /// Call this once from Unity's main thread during initialization
        /// </summary>
        public static void InitializeMainThread()
        {
            _mainThreadId = Thread.CurrentThread.ManagedThreadId;
        }

        public static bool IsMainThread => _mainThreadId.HasValue && 
            Thread.CurrentThread.ManagedThreadId == _mainThreadId;

        public static T ExecuteSafe<T>(Func<T> nativeCall, string operationName)
        {
            if (!_mainThreadId.HasValue)
            {
                throw new InvalidOperationException("NativeInterop not initialized");
            }

            lock (_syncRoot)
            {
                try
                {
                    return nativeCall();
                }
                catch (Exception ex)
                {
                    throw new NativeInteropException(
                        $"Failed to execute native operation '{operationName}'", ex);
                }
            }
        }
    }

    public class NativeInteropException : Exception
    {
        public NativeInteropException(string message, Exception inner) 
            : base(message, inner) { }
    }
}
