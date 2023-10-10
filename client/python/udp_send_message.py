import socket;

def send_udp_message(message, server_ip, server_port):
    # Create UDP socket.
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        udp_socket.sendto(message.encode(), (server_ip, server_port))
        print(f"Sent message: {message}")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        # Close the socket.
        udp_socket.close()

if __name__ == "__main__":
    server_ip = "192.168.1.85"
    server_port = 54200
    message = "hello nih"

    send_udp_message(message, server_ip, server_port)
