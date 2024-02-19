#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "pio_i2c.h"
#include "ws2812.h"
#include "ws2812.pio.h"
#include "registers.h"

const int addr = 0x39;  // APDS9960 I2C address
PIO pio = pio0;
uint sm = 0;

// Function prototypes
void write_register(uint8_t reg, uint8_t value);
void APDS9960_init();
void APDS9960_setProximityGain(uint8_t gain);
void APDS9960_setLEDDrive(uint8_t drive);
void APDS9960_setProximityPulse(uint8_t pulses, uint8_t length);
uint8_t APDS_read_proximity();

// Function to write a byte to a register
void write_register(uint8_t reg, uint8_t value) {
    uint8_t buf[] = {reg, value};
    pio_i2c_write_blocking(pio, sm, addr, buf, 2, false);
}

// Initialize APDS9960 for proximity sensing
void APDS9960_init() {
    write_register(0x80, 0x05);  // Enable power and proximity sensor
    APDS9960_setProximityGain(2);  // Set proximity gain to 4x
    APDS9960_setLEDDrive(0);  // Set LED drive strength to 100mA
    APDS9960_setProximityPulse(32, 2);  // Set 32 pulses with length 2
}

// Set the proximity gain
void APDS9960_setProximityGain(uint8_t gain) {
    uint8_t reg = 0x8F;  // Proximity control register address
    uint8_t value = (gain & 0x03) << 2;  // Gain value is in bits [3:2]
    write_register(reg, value);
}

// Set the LED drive strength
void APDS9960_setLEDDrive(uint8_t drive) {
    uint8_t reg = 0x8F;  // Proximity control register address
    uint8_t value = (drive & 0x03);  // Drive strength value is in bits [1:0]
    write_register(reg, value);
}

// Set the proximity pulse count and length
void APDS9960_setProximityPulse(uint8_t pulses, uint8_t length) {
    uint8_t reg = 0x8E;  // Proximity pulse count register
    uint8_t value = ((length & 0x07) << 6) | (pulses & 0x3F);
    write_register(reg, value);
}

// Read proximity data from APDS9960
uint8_t APDS_read_proximity() {
    uint8_t reg = 0x9C;  // Proximity data register address
    uint8_t proximity = 0;
    pio_i2c_write_blocking(pio, sm, addr, &reg, 1, true);
    pio_i2c_read_blocking(pio, sm, addr, &proximity, 1, false);
    return proximity;
}
#define SWITCH_PIN 28
int main() {
    const uint SDA_PIN = 22;
    const uint SCL_PIN = 23;
    
    stdio_init_all();
    // gpio_init(SWITCH_PIN);
    // gpio_set_dir(SWITCH_PIN, GPIO_IN);
    // gpio_pull_up(SWITCH_PIN); // Enable internal pull-up resistor

  
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, SDA_PIN, SCL_PIN);
    APDS9960_init();

    while (true) {     
        uint8_t proximity = APDS_read_proximity();
        printf("Proximity: %d\n", proximity);
        sleep_ms(500);
        // bool switch_state = gpio_get(SWITCH_PIN) == 0; // Switch is active low
        // printf("Is the switch on? ", switch_state); // Corrected formatting
        // sleep_ms(500);
    }
}
