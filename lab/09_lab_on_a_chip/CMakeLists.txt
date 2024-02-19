cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

#include(pico_extras_import_optional.cmake)

# Use ARM GCC Toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/AbdiMPC/Desktop/ESE5190/pico/pico-sdk/cmake/preload/toolchains/pico_arm_gcc.cmake" CACHE STRING "Toolchain file")


project(lab9_project C CXX ASM)
include_directories(${CMAKE_CURRENT_LIST_DIR} ${CMAKE_BINARY_DIR})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)



# Initialize the SDK
pico_sdk_init()

#include(example_auto_set_url.cmake)
# Your source file     


    
add_executable(lab9
    lab9.c
    ws2812.c
    apds_registers.h
    pio_i2c.c
    pio_i2c.h
    neopixel.c
    
    # Add any additional source files here
)

# Generate PIO header
pico_generate_pio_header(lab9 ${CMAKE_CURRENT_LIST_DIR}/i2c.pio)
pico_generate_pio_header(lab9 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)


# Pull in our pico_stdlib which aggregates commonly used features
#target_sources(lab1 PRIVATE lab1.c neopixel.c) # CMAKE -> C (compiler)
target_link_libraries(lab9 pico_stdlib hardware_pio hardware_dma hardware_i2c pico_multicore)


pico_enable_stdio_usb(lab9 1) # PICO SDK (USB)
pico_enable_stdio_uart(lab9 0) # PICO SDK (USB)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(lab9)