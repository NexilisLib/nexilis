import socket

def send_specific_bytes_via_udp(ip_address, port):
    # Create a bytes object with specific data (0x10 and 0x10)
    data = bytes([0x10, 0x10])

    # Create a UDP socket
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        # Send the bytes to the specified IP address and port
        udp_socket.sendto(data, (ip_address, port))

        # Receive a response from the server (optional)
        response, server = udp_socket.recvfrom(1024)
        print("Received response:", response.decode())

    finally:
        # Close the socket
        udp_socket.close()

# Example usage:
ip = 'localhost'  # Replace with the destination IP address
port = 54200  # Replace with the destination port number

send_specific_bytes_via_udp(ip, port)
