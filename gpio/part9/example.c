/*

  Simple gpiod example of toggling three LEDs connected to GPIO lines.
  Exits when pushbutton connected to GPIO line is pressed.  Written
  for Raspberry Pi platform with LEDs on GPIO pins 24, 25, and 5 and
  pushbutton on GPIO 6.

*/

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  const char *chipname = "gpiochip0";
  struct gpiod_chip *chip;
  struct gpiod_line *lineRed;    // Red LED
  struct gpiod_line *lineGreen;  // Green LED
  struct gpiod_line *lineYellow; // Yellow LED
  struct gpiod_line *lineButton; // Pushbutton
  int i, ret, val;

  // Open GPIO chip
  chip = gpiod_chip_open_by_name(chipname);
  if (!chip) {
    perror("Open chip failed\n");
    return 1;
  }

  // Open GPIO lines
  lineRed = gpiod_chip_get_line(chip, 24);
  if (!lineRed) {
    perror("Get line failed\n");
    return 1;
  }

  lineGreen = gpiod_chip_get_line(chip, 25);
  if (!lineGreen) {
    perror("Get line failed\n");
    return 1;
  }

  lineYellow = gpiod_chip_get_line(chip, 5);
  if (!lineYellow) {
    perror("Get line failed\n");
    return 1;
  }

  lineButton = gpiod_chip_get_line(chip, 6);
  if (!lineButton) {
    perror("Get line failed\n");
    return 1;
  }

  // Open LED lines for output
  ret = gpiod_line_request_output(lineRed, "example1", 0);
  if (ret < 0) {
    perror("Request line as output failed\n");
    return 1;
  }

  ret = gpiod_line_request_output(lineGreen, "example1", 0);
  if (ret < 0) {
    perror("Request line as output failed\n");
    return 1;
  }
  
  ret = gpiod_line_request_output(lineYellow, "example1", 0);
  if (ret < 0) {
    perror("Request line as output failed\n");
    return 1;
  }

  // Open switch line for input
  ret = gpiod_line_request_input(lineButton, "example1");
  if (ret < 0) {
    perror("Request line as input failed\n");
    return 1;
  }

  // Blink LEDs in a binary pattern
  i = 0;
  while (true) {
    ret = gpiod_line_set_value(lineRed, (i & 1) != 0);
    if (ret < 0) {
      perror("Set line output failed\n");
      return 1;
    }
    ret = gpiod_line_set_value(lineGreen, (i & 2) != 0);
    if (ret < 0) {
      perror("Set line output failed\n");
      return 1;
    }
    ret = gpiod_line_set_value(lineYellow, (i & 4) != 0);
    if (ret < 0) {
      perror("Set line output failed\n");
      return 1;
    }
    
    val = gpiod_line_get_value(lineButton);
    if (val < 0) {
      perror("Read line input failed\n");
      return 1;
    }

    // Exit if button pressed
    if (val == 0) {
      break;
    }

    usleep(100000);
    i++;
  }

  // Release lines and chip
  gpiod_line_release(lineRed);
  gpiod_line_release(lineGreen);
  gpiod_line_release(lineYellow);
  gpiod_line_release(lineButton);
  gpiod_chip_close(chip);
  return 0;
}
