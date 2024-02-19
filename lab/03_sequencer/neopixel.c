// neopixel.c
#include "neopixel.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "hardware/gpio.h"
#include <stdint.h>
#include <stdbool.h>

void neopixel_init() {
    PIO pio = pio0;
    uint32_t sm = 0;  // Assuming SM 0 is used
    uint32_t offset = pio_add_program(pio, &ws2812_program);
    
    // Initialize and turn on power to Neopixel LED
    const uint32_t NEOPIXEL_POWER_PIN = 11;
    gpio_init(NEOPIXEL_POWER_PIN);
    gpio_set_dir(NEOPIXEL_POWER_PIN, GPIO_OUT);
    gpio_put(NEOPIXEL_POWER_PIN, 1);  // Set pin high to turn on power

    ws2812_program_init(pio, sm, offset, 12 /* NEOPIXEL_PIN */, 800000, false);
}

void neopixel_set_rgb(uint32_t rgb) {
    PIO pio = pio0;
    uint32_t sm = 0;  // Assuming SM 0 is used
    uint32_t grb = ((rgb & 0xFF0000) >> 8) | ((rgb & 0x00FF00) << 8) | (rgb & 0x0000FF);
    pio_sm_put_blocking(pio, sm, grb << 8u);
}
