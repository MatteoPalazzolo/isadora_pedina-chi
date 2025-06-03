import serial
import time

arduino = serial.Serial(port='COM7', baudrate=9600, timeout=5)

time.sleep(2)

while True:
    if arduino.in_waiting > 0:
        linea: bytes = arduino.readline()
        print("Raw:", linea)
        print("Ricevuto:", linea.decode('utf-8').strip())
