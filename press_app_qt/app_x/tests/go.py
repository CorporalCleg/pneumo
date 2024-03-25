import socket
import serial
import time
import random
import json

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

def send():

    time.sleep(0.1)
    ser.reset_input_buffer()
    measure = ser.readline().decode('utf-8').rstrip().lstrip('(').rstrip(')')
    #measure = f'{random.random():.2f} 3.1 2.2 4.4'
    print(measure)


while True:
    send()