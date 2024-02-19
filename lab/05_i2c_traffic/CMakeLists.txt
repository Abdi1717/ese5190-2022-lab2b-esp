cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

#include(pico_extras_import_optional.cmake)

# Use ARM GCC Toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/AbdiMPC/Desktop/ESE5190/pico/pico-sdk/cmake/preload/toolchains/pico_arm_gcc.cmake" CACHE STRING "Toolchain file")


project(lab5_project C CXX ASM)
include_directories(${CMAKE_CURRENT_LIST_DIR} ${CMAKE_BINARY_DIR})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)



# Initialize the SDK
pico_sdk_init()

#include(example_auto_set_url.cmake)
# Your source file     


    
add_executable(lab5
    lab5.c
    ws2812.c
    # Add any additional source files here
)

# Generate PIO header
pico_generate_pio_header(lab5 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)


# Pull in our pico_stdlib which aggregates commonly used features
#target_sources(lab1 PRIVATE lab1.c neopixel.c) # CMAKE -> C (compiler)
target_link_libraries(lab5 pico_stdlib hardware_pio)


pico_enable_stdio_usb(lab5 1) # PICO SDK (USB)
pico_enable_stdio_uart(lab5 0) # PICO SDK (USB)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(lab5)