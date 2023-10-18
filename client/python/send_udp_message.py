import socket

# UDP server address and port
server_address = ('localhost', 54200)

# Message to be sent
message = "Hello, UDP Server!"

# Create a UDP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    # Send data to the server
    client_socket.sendto(message.encode(), server_address)

    # Receive a response from the server (optional)
    # response, server = client_socket.recvfrom(1024)
    # print("Received response:", response.decode())

finally:
    # Close the socket
    client_socket.close()
