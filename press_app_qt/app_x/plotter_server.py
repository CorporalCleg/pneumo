import socket


class p_server:
    def __init__(self):
        self.server = socket.socket()
        self.port = 1414
        self.hostname = '0.0.0.0'

        self.server.bind((self.hostname, self.port))
        self.server.listen(5)


    def send(self, measure="0.0", target="0.0"):

        message = (measure + ' ' + target).encode()
        print('server connecting..')
        self.conn, _ = self.server.accept()
        self.conn.send(message)
        self.conn.close()
        print(f'server send: {measure}')