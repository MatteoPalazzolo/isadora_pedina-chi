import socket
import time

HOST = 'localhost'  # Puoi anche usare l'indirizzo IP del server
PORT = 12345        # Una porta qualsiasi sopra 1024

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    server_socket.bind((HOST, PORT))
    server_socket.listen()
    print(f"Server in ascolto su {HOST}:{PORT}...")
    conn, addr = server_socket.accept()
    with conn:
        print(f"Connesso da {addr}")
        while True:
            try:
                conn.sendall(b'0')
                time.sleep(1)  # invia uno 0 al secondo
            except (BrokenPipeError, ConnectionResetError):
                print("Connessione persa.")
                break
