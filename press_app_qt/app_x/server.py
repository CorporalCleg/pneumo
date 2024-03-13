import socket
import json
# server = socket.socket()
# hostname = socket.gethostname()
# port = 12345
# server.bind((hostname, port))
# server.listen(5)

# while True:
#     conn, _ = server.accept()
#     data = conn.recv(1024)
#     print(data.decode())
#     message = 'hello from server'
#     conn.send(message.encode())
#     conn.close()

class server():
    def __init__(self):
        self.server = socket.socket()
        self.hostname = socket.gethostname()
        self.port = 12345
        self.server.bind((self.hostname, self.port))
        self.server.listen(5)
        
    
    def recv(self):
        self.conn, _ = self.server.accept()
        data = self.conn.recv(1024)
        self.conn.close()
        return(data.decode())

    def send(self, target: dict):
        self.conn, _ = self.server.accept()
        data = json.dumps(target).encode()
        data = self.conn.send(data)
        self.conn.close()
        print('call')

        return