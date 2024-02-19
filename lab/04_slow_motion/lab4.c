#include <stdio.h>
#include <stdlib.h>
#include "ws2812.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include <stdint.h>
#include <stdbool.h>

#define COLOR_MODE_RGBW true
#define NEOPIXEL_PIN 12
#define NEOPIXEL_POWER_PIN 11
#define NEOPIXEL_PIO pio0
#define STATE_MACHINE 0
#define NEOPIXEL_FREQ 800000

#define COMMAND_RECORD 'r'
#define COMMAND_REPLAY 'p'
#define BUTTON_PIN 21

int main() {
    stdio_init_all();
    uint programOffset = pio_add_program(NEOPIXEL_PIO, &ws2812_program);
    ws2812_program_init(NEOPIXEL_PIO, STATE_MACHINE, programOffset, NEOPIXEL_PIN, NEOPIXEL_FREQ, COLOR_MODE_RGBW);
    activate_neopixel();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    uint32_t userInput = 0;
    uint32_t controlFlag = 0;

    while(true) {
        userInput = getchar_timeout_us(0);
        switch(userInput) {
            case COMMAND_RECORD:
                change_pixel_color(0X00FF0000); // Red for recording
                sleep_ms(1000);
                while(true) {
                    controlFlag = getchar_timeout_us(0);
                    if(!gpio_get(BUTTON_PIN)) {
                        printf("Button Pressed\n");
                        change_pixel_color(0X0000FF00); // Green when button pressed
                    } else {
                        printf("Button Released\n");
                        change_pixel_color(0x00000000); // Off when button not pressed
                    }
                    if(controlFlag == 'N') {
                        change_pixel_color(0X00000000); // Turn off
                        sleep_ms(10);
                        break;
                    }
                    sleep_ms(10); 
                }
                break;
                
            case COMMAND_REPLAY:
                while(true) {
                    controlFlag = getchar_timeout_us(0);
                    if(controlFlag == '1') {
                        change_pixel_color(0X000000FF); // Blue for replay '1'
                    } else if(controlFlag == '0') {
                        change_pixel_color(0x00000000); // Off for replay '0'
                    } else if(controlFlag == 'N') {
                        change_pixel_color(0x00000000); // Turn off at end
                        sleep_ms(10);
                        break;
                    }
                    sleep_ms(10);
                }
                break;
        }
    }
    return 0;
}
