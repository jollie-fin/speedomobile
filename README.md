# Introduction

This project goal is to have a fun plateform to showcase the following :

- sensor fusion
- persistence of vision
- spatial pollution measurement
- video stream enhancement
- non trivial Rust project
- PCB design

It consists, in a way, in a "let's put every components I can find" kind of project

# Firmware

To be done

# 3D printing

Some care will be taken to properly protect components from the rain

To be done

# Hardware

On the hardware side, four boards have been designed

## Main board

This board contain the main MCU, and display the following features :

- 9DOF IMU
- high resolution GPS (ublox M9N)
- Brake sensor
- Ambiant light measurement
- Wheel speed measurement
- Communication with TSDZ2 motor
- I2C expansion connector
- Communication with power board
- Connector for remote control buttons

This board could/should have been a 4 layer board, but I wanted to try the interesting challenge of providing low impedance ground connection with only two layer. Fortunately, besides the GPS antenna, every signal here is low frequency.

![Main view](./hardware/images%20for%20README/main-board-main.jpg 'main board')
![Top view](./hardware/images%20for%20README/main-board-top.png 'main board')
![Bottom view](./hardware/images%20for%20README/main-board-bottom.png 'main board')

## Power board

This board transforms and distributes high voltage from the battery to low 5V voltage.

It also :

- measures battery usage
- protects every connection through PTC fuses
- provides SPI connexion for APA102 led strip
- provides switched 5V connector

Carrying 72A over a 35um pcb is hard. My design choice is to solder copper braid over the back of the board to carry this current. That way, low impedance connection can be done at a cheap price.

![Main view](./hardware/images%20for%20README/power-board-main.jpg 'power board')
![Top view](./hardware/images%20for%20README/power-board-top.png 'power board')
![Bottom view](./hardware/images%20for%20README/power-board-bottom.png 'power board')

## Main I2C board

This board simply connect to the I2C bus the following :

- An I2C LCD (20x4)
- An RFID reader
- Two rotary encoder, through an I2C expander

This board will be the main way to interact with the main board, besides WIFI

![Main view](./hardware/images%20for%20README/i2c-board-main.jpg 'i2c board')
![Top view](./hardware/images%20for%20README/i2c-board-top.png 'i2c board')

## Air quality measurement I2C board

This board simply connect to the I2C bus the following :

- A Temperature/Pressure/Humidity/VOC BME680 sensor
- A PM2.5 sensor

It aims at providing high quality information about the air around the bike

![Main view](./hardware/images%20for%20README/i2c-board-measurement-main.jpg 'power board')
![Top view](./hardware/images%20for%20README/i2c-board-measurement-top.png 'power board')
![Bottom view](./hardware/images%20for%20README/i2c-board-measurement-bottom.png 'power board')
