/*

Example of programming GPIO from C or C++ using the WiringPi library
on a Raspberry Pi.

Will cycle through three LEDs in a binary pattern. If the pushbutton is
pressed, it will turn off all LEDs and exit.

Jeff Tranter <jtranter@ics.com>

*/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

int main(void)
{
    // Red LED: Physical pin 18, BCM GPIO24, and WiringPi pin 5.
    const int led1 = 5;

    // Green LED: Physical pin 22, BCM GPIO25, and WiringPi pin 6.
    const int led2 = 6;

    // Yellow LED: Physical pin 29, BCM GPIO5, and WiringPi pin 21.
    const int led3 = 21;

    // Switch: Physical pin 31, BCM GPIO6, and WiringPi pin 22.
    const int button = 22;

    wiringPiSetup();

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(button, INPUT);

    fprintf(stderr, "Running on Raspberry Pi revision %d\n", piBoardRev());

    while (1) {
        for (int i = 0; i < 8; i++) {
            if (i & 1) {
                digitalWrite(led1, HIGH);
            } else {
                digitalWrite(led1, LOW);
            }
            if (i & 2) {
                digitalWrite(led2, HIGH);
            } else {
                digitalWrite(led2, LOW);
            }
            if (i & 4) {
                digitalWrite(led3, HIGH);
            } else {
                digitalWrite(led3, LOW);
            }

            delay(100);
        }

        if (digitalRead(button) == LOW) {
            digitalWrite(led1, LOW);
            digitalWrite(led2, LOW);
            digitalWrite(led3, LOW);
            exit(1);
        }
    }
}
