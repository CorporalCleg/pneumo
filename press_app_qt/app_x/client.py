import socket
import random

measure = lambda: " ".join([str(random.randint(0, 10))] * 4)
client = socket.socket()
hostname = socket.gethostname()
port = 12346

client.connect((hostname, port))
message = measure().encode()

client.send(message)
data = client.recv(1024)
print(data.decode())
client.close()