#!/usr/bin/python

import RPi.GPIO as GPIO
import time
import atexit

def cleanup():
  print("Cleaning up")
  GPIO.cleanup()

led1 = 24
led2 = 25
led3 = 5
sw1 = 6

atexit.register(cleanup)

#print(GPIO.RPI_INFO)

print("Platform info:")
for key in GPIO.RPI_INFO:
  print("  {} = {}".format(key, GPIO.RPI_INFO[key]))

GPIO.setmode(GPIO.BCM)
#GPIO.setwarnings(False)
GPIO.setup(led1, GPIO.OUT)
GPIO.setup(led2, GPIO.OUT)
GPIO.setup(led3, GPIO.OUT)
GPIO.setup(sw1, GPIO.IN)

while True:
  # Make LEDs cycle in a binary count pattern.
  for i in range(8):
    if (i & 1):
      GPIO.output(led3, GPIO.HIGH)
    else:
      GPIO.output(led3, GPIO.LOW)
    if (i & 2):
      GPIO.output(led2, GPIO.HIGH)
    else:
      GPIO.output(led2, GPIO.LOW)
    if (i & 4):
      GPIO.output(led1, GPIO.HIGH)
    else:
      GPIO.output(led1, GPIO.LOW)

    time.sleep(0.2)

    # Wait if button is pressed (is high).
    while GPIO.input(sw1) == GPIO.LOW:
      pass
