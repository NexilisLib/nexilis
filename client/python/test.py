#!/usr/bin/python

from websocket import create_connection

try:
    ws = create_connection("ws://localhost:54201")
    print("WebSocket connection established.")

    while True:
        message = input("Enter a message to send (or 'exit' to quit): ")
        if message.lower() == 'exit':
            break

        ws.send(message)
        print("Sent: " + message)

        response = ws.recv()
        print("Received: " + response)

    ws.close()
    print("WebSocket connection closed.")
except Exception as e:
    print("Error:", e)
