/**
 * Tiny69_SDA5708_SPI.cpp
 * 
 * 
 */


#include "Arduino.h"
#include "SDA5708_SPI.h"
#include <avr/pgmspace.h>
#include "font_5x7.h"

// currently only for AVR
#if defined(__AVR__)

inline size_t SDA5708_SPI::write(uint8_t value) {
	send(value);
	return 1;
}

SDA5708_SPI::SDA5708_SPI(uint8_t load) {
    _loadpin = load;
    _brightnessval = 0;
    _col = 0; // Cursorpos
}

void SDA5708_SPI::init(){
	init_priv();
}

void SDA5708_SPI::init_priv()
{
    SPI.begin();
    SPI.beginTransaction(SPISettings(SDA5708_DEFAULT_SPI_FREQ, LSBFIRST, SPI_MODE0));

    pinMode(_loadpin, OUTPUT);
    digitalWrite(_loadpin, HIGH);

    clear();

    setCursor(0);
}

void SDA5708_SPI::clear() {
    writebyte(B11000111); // clear
    writebyte(B11100000 | _brightnessval);

    setCursor(0);
}

void SDA5708_SPI::brightness(uint8_t b) {
    _brightnessval = b & 0x0f;

    writebyte(B11100000 | _brightnessval );
}

void SDA5708_SPI::send(uint8_t value) {
    uint8_t *font_ptr;
    int idx;
    uint8_t pixelrow;
    uint8_t data;

    writebyte(_col | 0xa0);

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

        //Write char pattern 
        for (idx=0; idx < 7; idx++) {
            data = pgm_read_byte(font_ptr + idx);
            writebyte( data & 0x1f);
            // writebyte((*font_ptr) & 0x1f);
            // font_ptr++;
        }

        _col = (++_col) & 7;

    }
}

void SDA5708_SPI::writebyte(uint8_t value) {
    digitalWrite(_loadpin, LOW);
    SPI.transfer(value);
    digitalWrite(_loadpin, HIGH);
}

void SDA5708_SPI::setCursor(uint8_t col) {
    _col = col & 7;
    writebyte(_col | 0xa0);
}


#endif // end AVR

