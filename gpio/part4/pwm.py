#!/usr/bin/python

import RPi.GPIO as GPIO
import time
import atexit

def cleanup():
  print("Cleaning up")
  GPIO.cleanup()

atexit.register(cleanup)

led1 = 24
led2 = 25
led3 = 5

GPIO.setmode(GPIO.BCM)
GPIO.setup(led1, GPIO.OUT)
GPIO.setup(led2, GPIO.OUT)
GPIO.setup(led3, GPIO.OUT)

while True:
  for pin in [led1, led2, led3]:
    p = GPIO.PWM(pin, 50)
    p.start(0)
    for dc in range(0, 101, 5):
      p.ChangeDutyCycle(dc)
      time.sleep(0.05)
    for dc in range(100, -1, -5):
      p.ChangeDutyCycle(dc)
      time.sleep(0.05)
