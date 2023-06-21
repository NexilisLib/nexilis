import socket
import threading

def send_udp_message(message, host='localhost', port=1999):
    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        # Send the message to the specified host and port
        sock.sendto(message, (host, port))
        print("UDP Message sent successfully.")
    except socket.error as e:
        print("UDP Error sending message:", str(e))
    finally:
        # Close the socket
        sock.close()

def listen_messages(server_address, server_port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.bind((server_address, server_port))

    print(f"Listening for messages on {server_address}:{server_port}")

    while True:
        data, address = client_socket.recvfrom(1024)
        message = data.decode()
        print(f"Received message from {address[0]}:{address[1]} - {message}")

def start_listening_thread(server_address, server_port):
    listen_thread = threading.Thread(target=listen_messages, args=(server_address, server_port))
    listen_thread.start()

def main():
    # Create a byte array with a single element containing 0x10
    message = bytearray([0x10])

    # Send the byte array as the message (ping)
    send_udp_message(message)

    start_listening_thread('127.0.0.1', 1999)

if __name__ == "__main__":
    main()
