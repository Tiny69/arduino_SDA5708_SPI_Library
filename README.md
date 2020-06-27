# Arduino Library For 8 Character Display SDA5708 using SPI Hardware

The intention of this Lib is to access the display using Arduinos hardware SPI instead of bitbanging everything as what most of the examples found on the internet are doing.

As this Display has no clue about a font and how characters look like it cannot be used as a typical character display right away, as would be the case with an HD44780. 
It's more like a 5x7 dot matrix graphic display having 8 characters, 30x7 so to say.

This means we need a font library providing 5x7 pixel character faces.

For each byte sent the `LOAD` line has to be activated seperatedly. 

A total of 8 bytes have to be sent for updateing a single character, that includes the position of the character in the first byte sent.

## Wiring

SDA5708|arduino
--------|-----------
1 - Vcc|Vcc
2 - /LOAD|digital Pin e.g. 10, + Pullup Resistor 10k
3 - DATA|MOSI
4 - SDCLK|SCK
5 - /RESET|Pullup Resistor 10k or as well digital Pin for software controlled Reset
6 - Gnd|Gnd
_ |MISO -> **not connected !** as there is no data sent by the display

Data and SDCLK do not need pullup/down resistor as they are pulled from the AVR.

## Resources

Here you'll find more info about the Display:
https://www.sbprojects.net/knowledge/footprints/sda5708/index.php

font found here:
https://os.mbed.com/users/wim/code/SDA5708//file/fa31d04e35a4/SDA5708.cpp/
