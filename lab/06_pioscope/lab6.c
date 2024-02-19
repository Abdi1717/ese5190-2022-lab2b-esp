#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/structs/bus_ctrl.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

#include "ws2812.h"
#include "ws2812.pio.h"

const uint ANALYSIS_PIN_BASE = 22;
const uint ANALYSIS_PIN_COUNT = 2;
const uint SAMPLE_COUNT = 256;
const uint TRIGGER_ANALYSIS_PIN = 22;
const uint SYSTEM_BOOT_PIN = 21;

static inline uint calculateBitsPerWord(uint pinCount) {
    const uint SHIFT_REG_WIDTH = 32;
    return SHIFT_REG_WIDTH - (SHIFT_REG_WIDTH % pinCount);
}

void setupLogicAnalyser(PIO pio, uint sm, uint pinBase, uint pinCount, float div) {
    uint16_t captureProgramInstr = pio_encode_in(pio_pins, pinCount);
    struct pio_program captureProgram = {
            .instructions = &captureProgramInstr,
            .length = 1,
            .origin = -1
    };
    uint offset = pio_add_program(pio, &captureProgram);
    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_in_pins(&config, pinBase);
    sm_config_set_wrap(&config, offset, offset);
    sm_config_set_clkdiv(&config, div);
    sm_config_set_in_shift(&config, true, true, calculateBitsPerWord(pinCount));
    sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_RX);
    pio_sm_init(pio, sm, offset, &config);
}

void armLogicAnalyser(PIO pio, uint sm, uint dmaChannel, uint32_t *captureBuffer, size_t bufferSizeWords,
                      uint triggerPin, bool triggerLevel) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_restart(pio, sm);

    dma_channel_config channelConfig = dma_channel_get_default_config(dmaChannel);
    channel_config_set_read_increment(&channelConfig, false);
    channel_config_set_write_increment(&channelConfig, true);
    channel_config_set_dreq(&channelConfig, pio_get_dreq(pio, sm, false));

    dma_channel_configure(dmaChannel, &channelConfig,
        captureBuffer,        // Destination pointer
        &pio->rxf[sm],        // Source pointer
        bufferSizeWords,      // Number of transfers
        true                  // Start immediately
    );

    pio_sm_exec(pio, sm, pio_encode_wait_gpio(triggerLevel, triggerPin));
    pio_sm_set_enabled(pio, sm, true);
}

void displayCaptureBuffer(const uint32_t *buffer, uint pinBase, uint pinCount, uint32_t sampleCount) {
    printf("Capture Data:\n");
    uint recordSizeBits = calculateBitsPerWord(pinCount);
    for (int pin = 0; pin < pinCount; ++pin) {
        printf("%02d: ", pin + pinBase);
        for (int sample = 0; sample < sampleCount; ++sample) {
            uint bitIndex = pin + sample * pinCount;
            uint wordIndex = bitIndex / recordSizeBits;
            uint wordMask = 1u << (bitIndex % recordSizeBits + 32 - recordSizeBits);
            printf(buffer[wordIndex] & wordMask ? "-" : "_");
        }
        printf("\n");
    }
}

int main() {
    stdio_init_all();
    gpio_init(SYSTEM_BOOT_PIN);
    gpio_set_dir(SYSTEM_BOOT_PIN, GPIO_IN);
    initializeNeopixels();
    activateNeopixel();

    uint32_t neoPixelColor = 0x00A4B3E6;
    setPixelColor(neoPixelColor);
    sleep_ms(1000);
    setPixelColor(0x00000000);

    printf("PIO Logic Analyser Example\n");

    uint totalSampleBits = SAMPLE_COUNT * ANALYSIS_PIN_COUNT;
    totalSampleBits += calculateBitsPerWord(ANALYSIS_PIN_COUNT) - 1;
    uint bufferSizeWords = totalSampleBits / calculateBitsPerWord(ANALYSIS_PIN_COUNT);
    uint32_t *captureBuffer = malloc(bufferSizeWords * sizeof(uint32_t));
    hard_assert(captureBuffer);

    bus_ctrl_hw->priority = BUSCTRL_BUS_PRIORITY_DMA_W_BITS | BUSCTRL_BUS_PRIORITY_DMA_R_BITS;

    PIO pio = pio0;
    uint sm = 0;
    uint dmaChannel = 0;
    setupLogicAnalyser(pio, sm, ANALYSIS_PIN_BASE, ANALYSIS_PIN_COUNT, 256.f);

    while (true) {
        if (!gpio_get(SYSTEM_BOOT_PIN)) {

            printf("Preparing Analyser\n");
            armLogicAnalyser(pio, sm, dmaChannel, captureBuffer, bufferSizeWords, ANALYSIS_PIN_BASE + 1, false);

            gpio_set_function(ANALYSIS_PIN_BASE, GPIO_FUNC_I2C);
            gpio_set_function(ANALYSIS_PIN_BASE + 1, GPIO_FUNC_I2C);

            dma_channel_wait_for_finish_blocking(dmaChannel);
            displayCaptureBuffer(captureBuffer, ANALYSIS_PIN_BASE, ANALYSIS_PIN_COUNT, SAMPLE_COUNT);

            sleep_ms(1000);
        }
    }
}
