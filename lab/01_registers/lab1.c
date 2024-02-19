// lab1.c
#include "neopixel.h"
#include "hardware/regs/sio.h"
#include "hardware/structs/sio.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <stdbool.h>

#define BOOT_BUTTON_PIN 21

int main() {
    stdio_init_all();
    neopixel_init();

    while (true) {
        bool button_pressed = !(sio_hw->gpio_in & (1u << BOOT_BUTTON_PIN));
        if (button_pressed) {
            neopixel_set_rgb(0xFF0000); // Red
        } else {
            neopixel_set_rgb(0x000000); // Off
        }
        sleep_ms(250); // Debounce delay
    }
    return 0;
}
