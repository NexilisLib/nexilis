import threading
import asyncio
import websockets
import socket

class CommunicationClient:
    def __init__(self, ws_host="192.168.1.85", ws_port=54201, udp_host="0.0.0.0", udp_port=54200):
        self.ws_uri = f"ws://{ws_host}:{ws_port}"
        self.udp_host = udp_host
        self.udp_port = udp_port
        self.ws_thread = None
        self.udp_thread = None
        self.ws_socket = None
        self.udp_socket = None
        self.running = False

    async def websocket_communication(self):
        async with websockets.connect(self.ws_uri) as websocket:
            while self.running:
                message = input("Enter a message to send via WebSocket (or type 'exit' to quit): ")
                if message.lower() == 'exit':
                    self.stop()
                    break

                await websocket.send(message)
                print(f"Sent websocket message: {message}")

                response = await websocket.recv()
                print(f"Received response from Websocket server: {response}")

    def udp_listener(self):
        try:
            self.udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.udp_socket.bind((self.udp_host, self.udp_port))
            print(f"Listening messages from {self.udp_host} port {self.udp_port}")

            while self.running:
                data, addr = self.udp_socket.recvfrom(1024)
                print(f"Received UDP message {addr}: {data.decode('utf-8')}")

        except Exception as e:
            print(f"Error in UDP listener: {e}")

        finally:
            self.udp_socket.close()

    def start(self):
        self.running = True
        self.ws_thread = threading.Thread(target=lambda: asyncio.run(self.websocket_communication()))
        self.udp_thread = threading.Thread(target=self.udp_listener)

        self.ws_thread.start()
        self.udp_thread.start()

    def stop(self):
        self.running = False

        if self.ws_socket:
            self.ws_socket.close()

        if self.udp_socket:
            self.udp_socket.close()

if __name__ == "__main__":
    client = CommunicationClient()
    client.start()

    # Allowing the user to stop the client by typing 'exit'
    while client.running:
        pass



"""

async def websocket_communication():
    uri = "ws://192.168.1.85:54201"

    async with websockets.connect(uri) as websocket:
        message = bytearray([0x10])
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
"""
