
#include "DHT.h"
#include <SDA5708_SPI.h>

//#include <SPI.h>  // the lib does that now

#define DHTPIN 7
#define DHTTYPE DHT22

#define LOAD_OUT 10

byte brightnessVal[] = {0,1,2,3,4,5,6,9,10,11,12,13,14,15};
byte brightnessIdx = 0;

int analogPin = A5;
int poti = 0;

int loopcounter = 0; // for when we want new DHT values

// for reading
float h1 = 0;
float t1 = 0;
// for output
float h = 0;
float t = 0;

SDA5708_SPI led(LOAD_OUT);

DHT dht(DHTPIN, DHTTYPE);

void setup() {

// commented out because the lib takes care of it now in its init()
//  pinMode(LOAD_OUT, OUTPUT);
//  digitalWrite(LOAD_OUT, HIGH);

  pinMode(analogPin, INPUT);

  led.init(); // clear characters

  dht.begin();

//  delay(750);

//  led.setCursor(0);
//  led.print()
//  led.print("ABCDEFGH");
//  led.print("IJKLMNOP");
//  led.print("QRSTUVWX");
//  led.print("YZ012345");
//  led.print("6789!\"#$");   // backslash +1
//  led.print("%&'()*+,");
//  led.print("-./:;<=>");
//  led.print("?@[\\]^_`");   // backslash +1
//  led.print("abcdefgh");
//  led.print("ijklmnop");
//  led.print("qrstuvwx");
//  led.print("yz{|}~  ");
//  //led.print("äöüß€°§@");
// sample characters for non printables
//  led.print((char)0xE4); // ä
//  led.print((char)0xF6); // ö
//  led.print((char)0xFC); // ü
//  led.print((char)0xDF); // ß
//  led.print((char)0x80); // €
//  //led.print((char)0xB0); // °
//  led.print((char)0xC4); // Ä
//  led.print((char)0xD6); // Ö
//  led.print((char)0xDC); // Ü
//  led.print((char)0x90); // 
//  led.print((char)0x91); // 
//  led.print((char)0x92); // 
//  led.print((char)0x93); // 
//  led.print((char)0x94); // 
//  led.print((char)0x95); // 
//  led.print((char)0x96); // 
//  led.print((char)0x97); // 

}

void loop() {

  poti = analogRead(analogPin);

  brightnessIdx = poti/73;  // 1023 / 73 = 14, because we have 14 different values in the brightness array.

  brightnessIdx = ((brightnessIdx >= sizeof(brightnessVal)) ? sizeof(brightnessVal)-1 : brightnessIdx);

  led.brightness(brightnessVal[brightnessIdx] );  // brightness update every loop

  loopcounter++;  // for DHT reading

  // DHT reading only about every 5th loop
  if (loopcounter > 5) {
    loopcounter = 0;  // reset 

    led.clear();  // clear only when new values, to see it blinking (flashing)

    h1 = dht.readHumidity();
    t1 = dht.readTemperature();

    // when something went wrong while reading from DHT
    // it does not update the output values, prints (a quick) error message instead
    if (isnan(h1) || isnan(t1) ) {
      led.print(F("Fail DHT"));
    }
    else {
      h = h1;
      t = t1;
    }
  }

  // DHT values output every loop
  led.setCursor(0);  // 1st position

  led.print(h, 0);
  led.print("% ");
  led.print(t, 0);
  led.print((char)0xB0); // °
  led.print("C");

  delay(900);  // loops about every second

}
