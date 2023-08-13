import asyncio
import websockets
 
async def test():
    async with websockets.connect('ws://localhost:8000') as websocket:

        message = bytearray([0x10])
        await websocket.send(message)
        response = await websocket.recv()
        print(response)
 
asyncio.get_event_loop().run_until_complete(test())
