import socket

HOST = '0.0.0.0'
PORT = 7777

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Server TCP in ascolto su {HOST}:{PORT}...")

    conn, addr = s.accept()
    print(f"Connessione stabilita da {addr}")

    with conn:
        while True:
            
            conn.setblocking(0)
            try:
                data = conn.recv(1024)
                msg = data.decode('utf-8').strip()
                print(f"Ricevuto: {msg}")
            except BlockingIOError:
                ...
                
            conn.sendall((input(">> ")+"\n").encode('utf-8'))