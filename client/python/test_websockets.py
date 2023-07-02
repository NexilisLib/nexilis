import asyncio
import websockets

url = "ws://localhost:8000"

async def connect_to_server(server_url):
    res = "Server " + server_url

    try:
        # Connect to WebSocket server
        async with websockets.connect(server_url) as websocket:
            print(res + " is up!")

            # Send a message to the WebSocket server
            message = bytearray([0x10])  # Create a message with a single byte value of 0x10
            await websocket.send(message)  # Send the message to the server
            print("Message sent successfully!")
    except:
        print(res + " is not up!")

def main():
    asyncio.run(connect_to_server(url))

if __name__ == "__main__":
    main()
