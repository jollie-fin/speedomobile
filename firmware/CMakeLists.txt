cmake_minimum_required(VERSION 3.13)
add_definitions(-DARDUINO=500)
set (CMAKE_CXX_STANDARD 20)

add_compile_definitions(PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=0)
set(PICO_SDK_PATH ../pico-sdk)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(speedomobile CXX C ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(fakearduino PN532/PN532 PN532/PN532_I2C hd44780)

add_executable(speedomobile
    main.cpp
    dht.cpp
    rfid.cpp
    lcd.cpp
    PN532/PN532_I2C/PN532_I2C.cpp
    PN532/PN532/PN532.cpp
    fakearduino/Wire.cpp
    fakearduino/Print.cpp
    fakearduino/Arduino.cpp
    hd44780/hd44780.cpp
)

pico_enable_stdio_uart(speedomobile 0)
pico_enable_stdio_usb(speedomobile 1)


# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(speedomobile
                        pico_stdlib
                        pico_cyw43_arch_none
                        hardware_spi
                        hardware_i2c
                        pico_stdio
                        pico_base
                        pico_time
                        pico_multicore
                        pico_stdio_usb)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(speedomobile)

# rest of your project