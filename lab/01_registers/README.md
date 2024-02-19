### TODO:

Toggle the Qt Py's Neopixel LED when the BOOT button is pressed, using only direct register reads to access the boot button status. You may use the SDK to initialize the board and the pins, and the WS2812 example code to toggle the Neopixel. 

### Overview:

By toggling the BOOT button, the code reads the status of the register's  with the button press and the Qt Py's Neopixel LED is turned on by writing a color to the registers. 

![](IMG_2452.gif)