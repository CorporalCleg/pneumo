import socket
import time
import random


hostname = socket.gethostname()
port = 12345

def send():

    time.sleep(0.5)
    client = socket.socket()
    client.connect((hostname, port))

    measure = str(random.random())
    print(measure)
    message = measure.encode()
    client.send(message)
    client.close()


while True:
    send()