import socket

def send_udp_message(message, host='localhost', port=1234):
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

def main():
    # Create a byte array with a single element containing 0x10
    message = bytearray([0x10])

    # Send the byte array as the message (ping)
    send_udp_message(message)

if __name__ == "__main__":
    main()
