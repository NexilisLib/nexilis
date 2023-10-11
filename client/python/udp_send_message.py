import socket;

def send_udp_message(message, server_socket_path):
    # Create UDP socket.
    udp_socket = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)

    try:
        udp_socket.sendto(message.encode(), server_socket_path)
        print(f"Sent message: {message}")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        # Close the socket.
        udp_socket.close()

if __name__ == "__main__":
    server_socket_path = '/tmp/nexilis'
    message = "hello nih"

    send_udp_message(message, server_socket_path)
