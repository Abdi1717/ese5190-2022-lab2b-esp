#include "pio_i2c.h"

// Define bit positions for I2C command structure
const int I2C_CMD_SHIFT = 10;
const int I2C_STOP_BIT_SHIFT = 9;
const int I2C_DATA_SHIFT = 1;
const int I2C_ACK_FLAG_SHIFT = 0;

// Check for I2C communication errors
bool hasI2CError(PIO pio, uint sm) {
    return pio_interrupt_get(pio, sm);
}

// Clear error state and resume I2C operation
void clearI2CError(PIO pio, uint sm) {
    pio_sm_drain_tx_fifo(pio, sm);
    pio_sm_exec(pio, sm, pio->sm[sm].execctrl & PIO_SM0_EXECCTRL_WRAP_BOTTOM_BITS);
    pio_interrupt_clear(pio, sm);
}

// Enable or disable automatic RX FIFO push
void toggleI2CRX(PIO pio, uint sm, bool enable) {
    uint32_t mask = PIO_SM0_SHIFTCTRL_AUTOPUSH_BITS;
    if (enable) {
        pio->sm[sm].shiftctrl |= mask;
    } else {
        pio->sm[sm].shiftctrl &= ~mask;
    }
}

// Write data to the I2C TX FIFO, handling full FIFO condition
void writeI2CFIFO(PIO pio, uint sm, uint16_t data) {
    while (pio_sm_is_tx_fifo_full(pio, sm)); // Wait for space in FIFO
    *(volatile uint16_t *)&pio->txf[sm] = data; // Write data to FIFO
}

// Attempt to write to I2C, aborting on error
void writeI2CData(PIO pio, uint sm, uint16_t data) {
    if (!hasI2CError(pio, sm)) {
        writeI2CFIFO(pio, sm, data);
    }
}

// Read a byte from the I2C RX FIFO
uint8_t readI2CByte(PIO pio, uint sm) {
    return (uint8_t)pio_sm_get(pio, sm);
}

// Initiate I2C start condition
void startI2CCondition(PIO pio, uint sm) {
    writeI2CData(pio, sm, 1u << I2C_CMD_SHIFT); // Start condition command
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_HIGH_SDA_LOW]);
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_LOW_SDA_LOW]);
}

// Initiate I2C stop condition
void stopI2CCondition(PIO pio, uint sm) {
    writeI2CData(pio, sm, 2u << I2C_CMD_SHIFT); // Stop condition command
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_LOW_SDA_LOW]);
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_HIGH_SDA_LOW]);
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_HIGH_SDA_HIGH]);
}

// Initiate I2C repeated start condition
void repeatedStartI2CCondition(PIO pio, uint sm) {
    writeI2CData(pio, sm, 3u << I2C_CMD_SHIFT); // Repeated start command
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_LOW_SDA_HIGH]);
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_HIGH_SDA_HIGH]);
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_HIGH_SDA_LOW]);
    writeI2CData(pio, sm, set_scl_sda_program_instructions[I2C_SCL_LOW_SDA_LOW]);
}

// Wait until I2C operation is complete or an error occurs
void waitForI2CIdle(PIO pio, uint sm) {
    while (!pio_sm_is_tx_fifo_empty(pio, sm) && !hasI2CError(pio, sm));
}

// Write data to I2C device, blocking until complete or error
int writeI2C(PIO pio, uint sm, uint8_t addr, const uint8_t *data, size_t len, bool noStop) {
    int result = 0;
    startI2CCondition(pio, sm);
    writeI2CFIFO(pio, sm, addr << 1); // Address with write bit

    for (size_t i = 0; i < len && !hasI2CError(pio, sm); i++) {
        writeI2CFIFO(pio, sm, data[i] << I2C_DATA_SHIFT);
    }

    if (!noStop) {
        stopI2CCondition(pio, sm);
    }
    waitForI2CIdle(pio, sm);

    if (hasI2CError(pio, sm)) {
        result = -1; // Indicate error
        clearI2CError(pio, sm);
    }

    return result;
}

// Read data from I2C device, blocking until complete or error
int readI2C(PIO pio, uint sm, uint8_t addr, uint8_t *data, size_t len, bool noStop) {
    int result = 0;
    repeatedStartI2CCondition(pio, sm);
    writeI2CFIFO(pio, sm, (addr << 1) | 1); // Address with read bit

    for (size_t i = 0; i < len && !hasI2CError(pio, sm); i++) {
        data[i] = readI2CByte(pio, sm);
    }

    if (!noStop) {
        stopI2CCondition(pio, sm);
    }
    waitForI2CIdle(pio, sm);

    if (hasI2CError(pio, sm)) {
        result = -1; // Indicate error
        clearI2CError(pio, sm);
    }

    return result;
}
