import socket
import time
import random


class plotter_client:
    def __init__(self):
        self.port = 12345
        self.hostname = socket.gethostname()
        

    def send(self, measure="0.0", target="0.0"):

        measure = str(random.random())
        target = str(random.random())
        time.sleep(0.5)
        self.client = socket.socket()
        self.client.connect((self.hostname, self.port))
        message = (measure + ' ' + target).encode()
        self.client.send(message)
        self.client.close()

if __name__ == '__main__':
    pcl = plotter_client()

    while 1:
        pcl.send()