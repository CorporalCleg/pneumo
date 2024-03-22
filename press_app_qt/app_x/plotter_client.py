import socket
import time
import random


class plotter_client:
    def __init__(self):
        self.port = 1414
        self.hostname = 'localhost'
        self.server_addr = (self.hostname, self.port)
        

    def send(self, measure="0.0", target="0.0"):

        #client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client = socket.socket()
        print(self.hostname)
        client.connect(self.server_addr)
        print(f'message = {message}')
        message = (measure + ' ' + target).encode()
        client.send(message)
        client.close()

if __name__ == "__main__":
    clt = plotter_client()
    while True:
        clt.send()