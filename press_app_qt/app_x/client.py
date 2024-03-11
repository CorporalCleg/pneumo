import socket
import random
import serial

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
ser.reset_input_buffer()

ser.write(b"set_pressure 22\n")
line = ser.readline().decode('utf-8').rstrip()

measure = line
client = socket.socket()
hostname = socket.gethostname()
port = 12345

client.connect((hostname, port))
message = measure().encode()

client.send(message)
data = client.recv(1024)
print(data.decode())
client.close()