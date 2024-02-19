#include "pico/stdlib.h"
#include <stdio.h>
#include "neopixel.h"
#include "hardware/gpio.h"

// Constants for GPIO pin and NeoPixel initialization
#define GPIO_BOOT_PIN 21

// Structure for holding register information
typedef struct {
    uint32_t Address;
    uint32_t Value;
} RegisterInfo; 

// Function to display register information
void displayRegisterInfo(RegisterInfo regInfo) {
    printf("--------------------------------\n");
    printf("Register Address: 0x%08x\n", regInfo.Address);
    printf("Register Value:    0x%08x\n", regInfo.Value);
    printf("--------------------------------\n");
}  

int main() {
    stdio_init_all();
    gpio_init(GPIO_BOOT_PIN);
    gpio_set_dir(GPIO_BOOT_PIN, GPIO_IN);
    neopixel_init();

    RegisterInfo currentRegInfo;
    currentRegInfo.Address = 0x00000000;
    currentRegInfo.Value = 0x00000000;

    uint32_t regAddressInput;
    uint32_t regValueInput;
    volatile uint32_t *regAddressPointer;

    while (true) {
        sleep_ms(5000); // Wait time before starting the loop
        printf("Enter register address (hex): ");
        scanf("%x", &regAddressInput);

        regAddressPointer = (volatile uint32_t *)regAddressInput;
        currentRegInfo.Address = regAddressInput;
        currentRegInfo.Value = *regAddressPointer;

        displayRegisterInfo(currentRegInfo); // Display the current register info

        printf("Enter 1 to write to this address: ");
        int userChoice = getchar();

        if(userChoice == '1') {
            printf("Enter new value for this register (hex): ");
            scanf("%x", &regValueInput);
            *regAddressPointer = regValueInput; // Update the register value
            currentRegInfo.Value = *regAddressPointer;

            displayRegisterInfo(currentRegInfo); // Display updated register info
        }
    }
    return 0;
}
