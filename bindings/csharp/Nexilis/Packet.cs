using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class Packet
    {
        private static NxLogger _logger = new NxLogger("Packet");
        public static void InitializeLogger(Action<Logger.LogLevel, string> logCallback)
        {
            _logger.AddHandler(logCallback);
            _logger.SetMinimumLevel(Logger.LogLevel.DEBUG);
        }

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_info_general();

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_info_clients();

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern RawNxData nexilis_packet_info_rooms();
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern void nexilis_packet_info_rooms_out(ref RawNxData result);

        public static NxData InfoRooms()
        {
            return NativeInterop.ExecuteSafe(() =>
            {
                var raw = nexilis_packet_info_rooms();
                if (raw.data == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Failed to get packet info rooms.");
                }
                return NxData.Create(raw);
            }, "InfoRooms");
        }
    }
}