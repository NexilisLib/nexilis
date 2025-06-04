namespace Nexilis
{
    public class Waiter : IDisposable
    {
        IntPtr _promiseHandle;
        readonly IntPtr _clientApiHandle;

        public Waiter(IntPtr clientApiHandle)
        {
            _clientApiHandle = clientApiHandle;
            _promiseHandle = WaiterNative.nexilis_promise_create();
        }

        public async Task WaitUntilRoomsCreated()
        {
            IntPtr waiterHandle = WaiterNative.nexilis_waiter_create_for_rooms_created(_clientApiHandle, _promiseHandle);

            await Task.Run(() =>
            {
                WaiterNative.nexilis_waiter_wait(waiterHandle);
            });

            if (waiterHandle != IntPtr.Zero)
            {
                WaiterNative.nexilis_waiter_destroy(waiterHandle);
            }
        }

        public void Dispose()
        {
            if (_promiseHandle != IntPtr.Zero)
            {
                WaiterNative.nexilis_promise_destroy(_promiseHandle);
                _promiseHandle = IntPtr.Zero;
            }
        }

        ~Waiter()
        {
            Dispose();
        }
    }
}
