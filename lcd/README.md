lcd - UC1601s I2C driver
========================

Wiring
------
 - CS0, CD, WR0, WR1: low
 - D0: SCK (pin5)
 - D3: SDA (pin 3)
 - D6, D7: high
 - BM1: low
 - BM2: high
 - rest of Dx: high or low, does not matter
 - rest of pins as on picture
In my case backlight is connected to +5V by 2 x 5kOhm paralell resistor

Kernel
------
You need to insert i2c-dev module to enable I2C bus
Then using i2c-tools backage you can detect if driver is visible:
> i2cdetect -y 1

Files
-----
 - font.h: font for displaying text, chip has no font installed
 - lcd.c: main driver
 - lcd.h: driver header file
 - logo.h: this file contains my logo for displaying;)
 - main.c: example usage
 - Makefile: as usual

Addressing
----------
Driver's pin CS0 defines I2C addressses of device (there are two: for command and data interface)
If it's connected low, the addresses will be 0x3c and 0x3d. If you change the address (by connecting pin high), change the addresses defined in lcd.c
