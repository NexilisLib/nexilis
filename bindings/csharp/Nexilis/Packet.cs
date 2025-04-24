using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public static class Packet
    {
        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_packet_info_general();

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_packet_info_clients();

        [DllImport(NativeLibrary.Name, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr nexilis_packet_info_rooms();

        public static NxData InfoGeneral()
        {
            var raw = nexilis_packet_info_general();
            if (raw == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to get packet info general.");
            }
            return NxData.FromHandle(raw);
        }
        public static NxData InfoClients()
        {
            var raw = nexilis_packet_info_clients();
            if (raw == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to get packet info clients.");
            }
            return NxData.FromHandle(raw);
        }
        public static NxData InfoRooms()
        {
            var raw = nexilis_packet_info_rooms();
            if (raw == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to get packet info rooms.");
            }
            return NxData.FromHandle(raw);
        }
    }
}