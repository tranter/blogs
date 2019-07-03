#!/usr/bin/python

import RPi.GPIO as GPIO
import time

led1 = 18

GPIO.setmode(GPIO.BOARD)
GPIO.setup(led1, GPIO.OUT)

while True:
    GPIO.output(led1, GPIO.HIGH)
    time.sleep(0.2)
    GPIO.output(led1, GPIO.LOW)
    time.sleep(0.2)
