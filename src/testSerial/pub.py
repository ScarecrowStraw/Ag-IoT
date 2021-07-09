import serial
import time
import sys
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

port = "/dev/ttyUSB0"

ser = serial.Serial(port, baudrate = 9600)

print ("starting")

while True:
	read_ser = ser.readline()
	print(read_ser)
	publish.single("demo/test", read_ser, hostname="127.0.0.1")
