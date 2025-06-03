import socket

HOST = 'localhost'  # Deve corrispondere a quello del server
PORT = 12345

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
    client_socket.connect((HOST, PORT))
    print("Connesso al server.")
    while True:
        data = client_socket.recv(1)
        if not data:
            break
        print(data.decode(), end='', flush=True)
