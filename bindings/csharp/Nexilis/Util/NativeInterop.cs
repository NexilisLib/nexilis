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

        /// <summary>
        /// Runs a native call that does not return a value. Serialized with
        /// the other interop calls so a native API is never entered
        /// concurrently from two managed threads.
        /// </summary>
        public static void ExecuteSafe(Action nativeCall, string operationName)
        {
            if (!_mainThreadId.HasValue)
            {
                throw new InvalidOperationException("NativeInterop not initialized");
            }

            lock (_syncRoot)
            {
                try
                {
                    nativeCall();
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
