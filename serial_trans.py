import serial
import sys



ser = serial.Serial('/dev/ttyACM0', 9600)

if ser.isOpen():
    while True:
        ser.write(b'a')
        while (ser.in_waiting == 0):
            print(ser.read())
            print(ser.in_waiting)
        print(ser.read())

else:
    print("Porta seriale non disponibile")