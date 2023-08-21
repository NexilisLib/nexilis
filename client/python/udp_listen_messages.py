import socket

def udp_listener():
    socket_host = "0.0.0.0"
    socket_port = 54200

    try:
        server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server.bind((socket_host, socket_port))
        print(f"Listening messages from {socket_host} port {socket_port}")

        while True:
            data, addr = server.recvfrom(1024)
            print(f"Received UDP message {addr}: {data.decode('utf-8')}")

    except Exception as e:
        print(f"Error in UDP listener: {e}")

    finally:
        server.close()

udp_listener()
