import socket

HOST = '192.168.0.101'
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    """
    while True:
        data = s.recv(1024)
        print('Ricevuto', repr(data.decode()))
    """