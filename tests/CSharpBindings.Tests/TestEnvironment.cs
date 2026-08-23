using System;

namespace Nexilis.Tests
{
    /// <summary>
    /// One-time initialization required by managed wrappers before use:
    /// native interop main-thread tracking and per-class NxLogger setup.
    /// </summary>
    public static class TestEnvironment
    {
        static bool _initialized;

        public static void Initialize()
        {
            if (_initialized)
            {
                return;
            }
            _initialized = true;

            NativeInterop.InitializeMainThread();

            Action<Logger.LogLevel, string> noop = (_, _) => { };
            Vector3<float>.InitializeLogger(noop);
            NxData.InitializeLogger(noop);
            Packet.InitializeLogger(noop);
        }
    }
}
