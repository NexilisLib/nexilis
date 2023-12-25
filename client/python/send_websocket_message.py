import threading
import asyncio
import websockets

async def websocket_communication():
    uri = "ws://localhost:54201"

    async with websockets.connect(uri) as websocket:
        message = bytearray([0x10, 0x10])
        await websocket.send(message)
        print(f"Sent websocket message: {message}")

        response = await websocket.recv()
        print(f"Received response from Websocket server: {response}")

def main():
    ws_thread = threading.Thread(target=lambda: asyncio.run(websocket_communication()))
    ws_thread.start()
    ws_thread.join()

if __name__ == "__main__":
    main()
