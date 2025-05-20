using System;
using System.Runtime.InteropServices;

namespace Nexilis.Client
{
    public class Room : IDisposable
    {
        IntPtr _roomPtr;

        public Room(IntPtr roomPtr)
        {
            _roomPtr = roomPtr;
        }

        public ulong GetId()
        {
            return RoomNative.nexilis_room_get_id(_roomPtr);
        }

        public void Dispose()
        {
        }
    }
}