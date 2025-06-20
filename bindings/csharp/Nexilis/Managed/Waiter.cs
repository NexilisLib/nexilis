namespace Nexilis
{
    public class Waiter : IDisposable
    {
        IntPtr _promiseHandle;
        readonly IntPtr _clientApiHandle;
        bool _disposed = false;

        public Waiter(IntPtr clientApiHandle)
        {
            _clientApiHandle = clientApiHandle;
            _promiseHandle = WaiterNative.nexilis_promise_create();

            if (_promiseHandle == IntPtr.Zero)
                throw new InvalidOperationException("Failed to create promise");
        }

        public async Task WaitUntilRoomsCreated()
        {
            if (_disposed) throw new ObjectDisposedException(nameof(Waiter));

            IntPtr waiterHandle = IntPtr.Zero;
            try
            {
                waiterHandle = WaiterNative.nexilis_waiter_create_for_rooms_created(_clientApiHandle, _promiseHandle);
                if (waiterHandle == IntPtr.Zero || !WaiterNative.nexilis_waiter_is_valid(waiterHandle))
                {
                    throw new InvalidOperationException("Failed to create valid waiter");
                }

                await Task.Run(() =>
                {
                    WaiterNative.nexilis_waiter_wait(waiterHandle);
                });

                // Check that the rooms actually exist.
                bool roomsExit = await VerifyRoomsExist();
                if (!roomsExit)
                {
                    throw new InvalidOperationException("Waiter completed but no rooms exist");
                }
            }
            finally
            {
                if (waiterHandle != IntPtr.Zero)
                {
                    WaiterNative.nexilis_waiter_destroy(waiterHandle);
                }
            }
        }


        async Task<bool> VerifyRoomsExist()
        {
            // Try three times with 500ms delay.
            for (int i = 0; i < 3; i++)
            {
                bool exists = await Task.Run(() =>
                {
                    try
                    {
                        var rooms = Client.ClientAPINative.nexilis_client_api_get_active_rooms(_clientApiHandle);
                        return rooms != IntPtr.Zero && Client.ClientAPINative.nexilis_client_api_rooms_count(_clientApiHandle) > 0;
                    }
                    catch
                    {
                        return false;
                    }
                });

                if (exists) return true;
                await Task.Delay(500);
            }
            return false;
        }

        public void Dispose()
        {
            if (!_disposed && _promiseHandle != IntPtr.Zero)
            {
                WaiterNative.nexilis_promise_destroy(_promiseHandle);
                _promiseHandle = IntPtr.Zero;
                _disposed = true;
            }
            GC.SuppressFinalize(this);
        }

        ~Waiter()
        {
            Dispose();
        }
    }
}
