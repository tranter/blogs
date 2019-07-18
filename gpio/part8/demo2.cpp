/*

Example of using libsoc for GPIO programming.

Toggles three LEDs in a "Cylon" pattern until the pushbutton is
pressed, and then exits. Will run on Raspberry Pi or Toradex Colibri
platforms.

This version reads the switch status using interrupts and a callback.

*/

#include <stdio.h>
#include <unistd.h>
#include "libsoc_gpio.h"
#include "libsoc_debug.h"

// Uncomment the appropriate lines below for the hardware it is
// runnning on.

// Raspberry Pi
//const int led1   = 24; // Red LED
//const int led2   = 25; // Green LED
//const int led3   =  5; // Yellow LED
//const int button =  6; // Pushbutton

// Toradex Colibri
const int led1   = 52; // Red LED
const int led2   = 53; // Green LED
const int led3   = 63; // Yellow LED
const int button = 93; // Pushbutton

// Delay (in microseconds)
const int delay = 100000;

// Exit flag
bool exitLoop = false;


// Callback invoked when button is pressed. Set flag to exit.
int buttonCallback(void* arg)
{
    printf("Button pressed\n");
    exitLoop = true;
    return 0;
}


// Main program
int main(void)
{
    // Uncomment the next line to enable debug output if desired.
    //libsoc_set_debug(1);

    // Request gpios. May need to use LS_SHARED with older versions
    // of libsoc.
    gpio *gpio_led1 = libsoc_gpio_request(led1, LS_GPIO_SHARED);
    gpio *gpio_led2 = libsoc_gpio_request(led2, LS_GPIO_SHARED);
    gpio *gpio_led3 = libsoc_gpio_request(led3, LS_GPIO_SHARED);
    gpio *gpio_button = libsoc_gpio_request(button, LS_GPIO_SHARED);

    // Ensure gpios were successfully requested.
    if (gpio_led1 == NULL) {
        printf("Failed gpio_led1 request.\n");
        exit(EXIT_FAILURE);
    }
    if (gpio_led2 == NULL) {
        printf("Failed gpio_led2 request.\n");
        exit(EXIT_FAILURE);
    }
    if (gpio_led3 == NULL) {
        printf("Failed gpio_led3 request.\n");
        exit(EXIT_FAILURE);
    }
    if (gpio_button == NULL) {
        printf("Failed gpio_button request.\n");
        exit(EXIT_FAILURE);
    }
  
    // Set directions.
    libsoc_gpio_set_direction(gpio_led1, OUTPUT);
    libsoc_gpio_set_direction(gpio_led2, OUTPUT);
    libsoc_gpio_set_direction(gpio_led3, OUTPUT);
    libsoc_gpio_set_direction(gpio_button, INPUT);
  
    // Check directions.
    if (libsoc_gpio_get_direction(gpio_led1) != OUTPUT) {
        printf("Failed to set direction of gpio_led1\n");
        exit(EXIT_FAILURE);
    }
    if (libsoc_gpio_get_direction(gpio_led2) != OUTPUT) {
        printf("Failed to set direction of gpio_led2\n");
        exit(EXIT_FAILURE);
    }
    if (libsoc_gpio_get_direction(gpio_led3) != OUTPUT) {
        printf("Failed to set direction of gpio_led3\n");
        exit(EXIT_FAILURE);
    }
    if (libsoc_gpio_get_direction(gpio_button) != INPUT) {
        printf("Failed to set direction of gpio_button\n");
        exit(EXIT_FAILURE);
    }

    // Set input edge to FALLING.
    libsoc_gpio_set_edge(gpio_button, FALLING);

    // Set up callback when button status changes.
    libsoc_gpio_callback_interrupt(gpio_button, &buttonCallback, (void *) 0);

    while (!exitLoop) {

        // Turn LEDs on and off in desired sequence.
        libsoc_gpio_set_level(gpio_led1, HIGH);
        libsoc_gpio_set_level(gpio_led2, LOW);
        libsoc_gpio_set_level(gpio_led3, LOW);
        usleep(delay);
 
        libsoc_gpio_set_level(gpio_led1, LOW);
        libsoc_gpio_set_level(gpio_led2, HIGH);
        libsoc_gpio_set_level(gpio_led3, LOW);
        usleep(delay);

        libsoc_gpio_set_level(gpio_led1, LOW);
        libsoc_gpio_set_level(gpio_led2, LOW);
        libsoc_gpio_set_level(gpio_led3, HIGH);
        usleep(delay);

        libsoc_gpio_set_level(gpio_led1, LOW);
        libsoc_gpio_set_level(gpio_led2, HIGH);
        libsoc_gpio_set_level(gpio_led3, LOW);
        usleep(delay);
    }

    // Cancel the callback as it is no longer needed.
    libsoc_gpio_callback_interrupt_cancel(gpio_button);

    // Turn all LEDs off.
    libsoc_gpio_set_level(gpio_led1, LOW);
    libsoc_gpio_set_level(gpio_led2, LOW);
    libsoc_gpio_set_level(gpio_led3, LOW);
  
    // Clean up.
    libsoc_gpio_free(gpio_led1);
    libsoc_gpio_free(gpio_led2);
    libsoc_gpio_free(gpio_led3);
    libsoc_gpio_free(gpio_button);
  
    return EXIT_SUCCESS;
}
