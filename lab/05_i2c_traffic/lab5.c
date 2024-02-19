#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"

int sensorAddress = 0x39;

static void initializeSensor() {
    // Setup the sensor with necessary initialization values
    uint8_t initValues[] = {0x80, 0x25};
    i2c_write_blocking(i2c1, sensorAddress, initValues, 2, false);
}

static void readSensorData(int8_t *data) {
    // Read a single byte from the sensor

    uint8_t reg = 0x9C; // Register to read from
    uint8_t buffer[1];

    i2c_write_blocking(i2c1, sensorAddress, &reg, 1, true);
    i2c_read_blocking(i2c1, sensorAddress, buffer, 1, false);

    *data = buffer[0];
}

int main() {
    const uint I2C_SDA_PIN = 22;
    const uint I2C_SCL_PIN = 23;
    const uint STATUS_LED_PIN = 29;

    stdio_init_all();

    // Initialize I2C communication
    i2c_init(i2c1, 200*1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));
    initializeSensor();

    // Initialize LED
    gpio_init(STATUS_LED_PIN);
    gpio_set_dir(STATUS_LED_PIN, GPIO_OUT);

    int8_t sensorData = 50;

    while (true) {     
        readSensorData(&sensorData);
        if (sensorData > 50) {
            gpio_put(STATUS_LED_PIN, 1); // Turn on LED
        } else {
            gpio_put(STATUS_LED_PIN, 0); // Turn off LED
        }
        sleep_ms(10);
    }
}
