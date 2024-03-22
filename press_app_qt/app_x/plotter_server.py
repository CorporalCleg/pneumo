import socket


class p_server:
    def __init__(self):
        self.server = socket.socket()
        self.port = 1414
        self.hostname = '0.0.0.0'

        self.server.bind((self.hostname, self.port))
        self.server.listen(5)


    def send(self, measure="0.0", target="0.0"):

        measure = str(random.random())
        print(measure)
        message = (measure + ' ' + target).encode()

        self.conn, _ = self.server.accept()
        self.conn.send(message)
        self.conn.close()

if __name__ == "__main__":
    import random
    import time
    
    srvr = p_server()
    while True:
        time.sleep(0.5)
        srvr.send()