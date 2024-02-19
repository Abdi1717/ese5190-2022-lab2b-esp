#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.h"
#include "ws2812.pio.h"

// Definitions for NeoPixel configuration
#define NEOPIXEL_PIO    pio0
#define STATE_MACHINE   0
#define NEOPIXEL_FREQ   800000
#define NEOPIXEL_PIN    12
#define POWER_CONTROL_PIN 11
#define COLOR_FORMAT    true  // true for RGBW, false for RGB

#define BOOT_BUTTON_PIN 21

// Structure to hold the flashlight state
typedef struct {
    uint32_t serialInput;
    uint32_t buttonState;
    uint32_t neoPixelColor;
} FlashlightState; 

int main() {
    stdio_init_all();
    gpio_init(BOOT_BUTTON_PIN);
    gpio_set_dir(BOOT_BUTTON_PIN, GPIO_IN);
    initialize_neopixels();
    activate_neopixel();

    FlashlightState flashlight;
    flashlight.serialInput =  0x00000000;
    flashlight.buttonState = 0x00000000;
    flashlight.neoPixelColor = 0x0012B4A6;

    change_neopixel_color(flashlight.neoPixelColor);
    sleep_ms(1000);
    change_neopixel_color(0x00000000);

    while(true) {
        int index = 0;
        uint32_t buttonPresses[1000];
        char userInput = 0;

        while(userInput != 'c') {
            printf("Press 'c' to start.\n");
            scanf(" %c", &userInput); // Added space to consume newline
        }
        printf("Recording button presses...\n");

        while (index < 1000) {
            flashlight.buttonState = gpio_get(BOOT_BUTTON_PIN) ? 0x00000000 : 0x00000001;
            buttonPresses[index] = flashlight.buttonState;

            change_neopixel_color(flashlight.buttonState ? flashlight.neoPixelColor : 0x00000000);
            sleep_ms(10);
            index++;
            printf("Time elapsed: %d ms\n", index * 10);
        }
        printf("Playback starts...\n");

        for (int replayIndex = 0; replayIndex < 1000; replayIndex++) {
            change_neopixel_color(buttonPresses[replayIndex] ? flashlight.neoPixelColor : 0x00000000);
            sleep_ms(10);
        }
        printf("Playback finished.\n");
    }
    return 0;
}
