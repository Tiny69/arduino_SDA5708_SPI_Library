/**
 * Tiny69_SDA5708_SPI.h
 * 
 */

#ifndef _Tiny69_SDA5708_SPI_H_
#define _Tiny69_SDA5708_SPI_H_

#include <inttypes.h>
#include "Print.h"
#include <SPI.h>

#if defined(__AVR__)
// Hardware SPI default speed
#define SDA5708_DEFAULT_SPI_FREQ 8000000L

// Class

class SDA5708_SPI : public Print {

public:

    // Constructor
    SDA5708_SPI(uint8_t load);
    void setCursor(uint8_t); 
    void init();
    void clear();
    void brightness(uint8_t);
    void graph(uint8_t, 
        uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    virtual size_t write(uint8_t);


private:
    void init_priv();
    void send(uint8_t);
    void writebyte(uint8_t);
    uint8_t _col;
    uint8_t _loadpin;
    uint8_t _brightnessval;
    
};



#endif // end AVR


#endif  // end _Tiny69_SDA5708_SPI_H_
