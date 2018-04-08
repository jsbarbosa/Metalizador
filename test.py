import RPi.GPIO as GPIO
import time

port = 4

GPIO.setmode(GPIO.BCM)

GPIO.setup(port, GPIO.OUT)

while True:
	try:
		GPIO.output(port, GPIO.HIGH)
		time.sleep(5)
		GPIO.output(port, GPIO.LOW)
		time.sleep(5)
	except:
		break
GPIO.cleanup()
