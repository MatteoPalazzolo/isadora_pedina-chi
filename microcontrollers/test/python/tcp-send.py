import socket

HOST = '127.0.0.1'
PORT = 6060

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    print(f"Connesso a {HOST}:{PORT}")

    while True:
        msg = input("Messaggio da inviare (invio per uscire): ")
        if not msg:
            break
        s.sendall((msg + '\n').encode('utf-8'))  # '\n' è utile se il server usa TCP Text Receive
