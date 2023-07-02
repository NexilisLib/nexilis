import asyncio
import websockets

url = "ws://localhost:8000"

async def connect_to_server(server_url):
    res = "Server " + server_url

    try:
        # Connect to Websocket server
        async with websockets.connect(server_url) as websocket:
            print(res + " is up!")
    except:
        print(res + " is not up!")


def main():
    asyncio.run(connect_to_server(url))

if __name__ == "__main__":
    main()
