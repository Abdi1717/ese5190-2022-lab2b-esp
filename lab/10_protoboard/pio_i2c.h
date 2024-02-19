#ifndef PIO_I2C_H
#define PIO_I2C_H

#include "i2c.pio.h"

// I2C Control Operations

// Initiates an I2C start condition on the specified PIO and state machine.
void initiateI2CStart(PIO pio, uint sm);

// Sends an I2C stop condition to terminate the communication.
void sendI2CStop(PIO pio, uint sm);

// Generates an I2C repeated start condition, enabling a new transfer without releasing the bus.
void generateI2CRepeatedStart(PIO pio, uint sm);

// Error Handling

// Checks if there's an error in the I2C communication.
bool hasI2CCommunicationError(PIO pio, uint sm);

// Clears any existing error state and attempts to resume normal I2C operation.
void clearI2CErrorAndResume(PIO pio, uint sm);

// I2C Data Transmission

// Attempts to transmit data over I2C, aborting if an error is detected.
void transmitI2CDataOrAbort(PIO pio, uint sm, uint16_t data);

// Retrieves a byte of data received over I2C.
uint8_t receiveI2CData(PIO pio, uint sm);

// High-level I2C Transactions

// Performs a blocking write operation over I2C, optionally without sending a stop condition.
int writeDataI2CBlocking(PIO pio, uint sm, uint8_t addr, const uint8_t *data, size_t length, bool noStop);

// Executes a blocking read operation from I2C, with an option to omit the stop condition.
int readDataI2CBlocking(PIO pio, uint sm, uint8_t addr, uint8_t *data, size_t length, bool noStop);

#endif // PIO_I2C_H
