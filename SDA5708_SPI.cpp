/**
 * Tiny69_SDA5708_SPI.cpp
 * 
 * extends Print class to use its print command
 * 
 */


#include "Arduino.h"
#include "SDA5708_SPI.h"
#include <avr/pgmspace.h>
#include "font_5x7.h"

// currently only for AVR
#if defined(__AVR__)

/* gets called from arduino print */
inline size_t SDA5708_SPI::write(uint8_t value) {
	send(value);
	return 1;
}

/* constructor of the class */
SDA5708_SPI::SDA5708_SPI(uint8_t load) {
    _loadpin = load;
    _brightnessval = 0;
    _col = 0; // Cursorpos
}

/* init LCD */ 
void SDA5708_SPI::init(){
	init_priv();
}

/* internal init command, just in case */
void SDA5708_SPI::init_priv()
{
    SPI.begin();
    SPI.beginTransaction(SPISettings(SDA5708_DEFAULT_SPI_FREQ, LSBFIRST, SPI_MODE0));

    pinMode(_loadpin, OUTPUT);
    digitalWrite(_loadpin, HIGH);

    clear();

    setCursor(0);
}

/* outputs the set clear command */
void SDA5708_SPI::clear() {
    writebyte(B11000111); // clear
    writebyte(B11100000 | _brightnessval);

    setCursor(0);
}

/* outputs the set brightness command */
void SDA5708_SPI::brightness(uint8_t b) {
    _brightnessval = b & 0x0f;  // only lower 4 bits used for brightness

    writebyte(B11100000 | _brightnessval );  // outputs brightness command
}

/* outputs binary data straight to the given character position */
void SDA5708_SPI::graph(uint8_t col, 
    uint8_t value0, uint8_t value1, uint8_t value2, uint8_t value3, 
    uint8_t value4, uint8_t value5, uint8_t value6) {

    setCursor(col);
    writebyte( value0 & 0x1f);  // character values need to have bits 5,6,7 = 0
    writebyte( value1 & 0x1f);
    writebyte( value2 & 0x1f);
    writebyte( value3 & 0x1f);
    writebyte( value4 & 0x1f);
    writebyte( value5 & 0x1f);
    writebyte( value6 & 0x1f);
}

/* gets called from print command */
void SDA5708_SPI::send(uint8_t value) {
    uint8_t *font_ptr;
    int idx;
    uint8_t data;

    writebyte(_col | 0xa0); // sets position

    // 'new line' and 'carriage return' sets position to 0
    if ((value == '\n') || (value == '\r')) {
        _col = 0;
    }
    else {
        if ((value < 0x20) ) {
            // Pointer to char pattern for 'space'
            font_ptr = (uint8_t *) font_5x7[0];     
        }  
        // else if ((value > 0x80)) {
        //     // Pointer to char pattern for 'space'
        //     font_ptr = (uint8_t *) font_5x7[0];     
        // }  
        else {          
            //value = value & 0x7f;
           // Pointer to char pattern 
            font_ptr = (uint8_t *) font_5x7[value - 0x20];
        }

        // Write char pattern 
        for (idx=0; idx < 7; idx++) {
            data = pgm_read_byte(font_ptr + idx);
            writebyte( data & 0x1f);
        }

        // calculates next position, when end of LCD is reached starts from 0
        _col = (++_col) & 7;  
    }
}

/* outputs a single byte to the LCD */
void SDA5708_SPI::writebyte(uint8_t value) {
    digitalWrite(_loadpin, LOW);
    SPI.transfer(value);
    digitalWrite(_loadpin, HIGH);
}

/* outputs the set position command */
void SDA5708_SPI::setCursor(uint8_t col) {
    _col = col & 7;
    writebyte(_col | 0xa0);
}


#endif // end AVR

