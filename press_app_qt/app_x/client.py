import socket
import serial
import time
import random
import json

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

hostname = socket.gethostname()
port = 12346


def send():

    time.sleep(0.1)
    client = socket.socket()
    client.connect((hostname, port))
    ser.reset_input_buffer()
    measure = ser.readline().decode('utf-8').rstrip().lstrip('(').rstrip(')')
    #measure = f'{random.random():.2f} 3.1 2.2 4.4'
    print(measure)
    message = measure.encode()
    client.send(message)
    data = client.recv(1024).decode()
    print(data)
    if data != '':
        data = json.loads(data)
        print(f'Got data {data}')
        target_message = f'set_pressure {list(data.values())[0]}'
        print(target_message)
        ser.write(target_message.encode())
        
    client.close()


while True:
    send()
