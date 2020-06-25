//We always have to include the library
#include <SPI.h>
#include "ArcadeBundle.h"

/*
 Now we need a ArcadeBundle to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 13 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
//LedControl lc=LedControl(11,13,10,1);
LedControl lc(10, 1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=100;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 1);
  /* and clear the display */
  lc.clearDisplay(0);

  randomSeed(analogRead(0));
}

void loop() { 
  //writeArduinoOnMatrix();
  //rows();
  //columns();
  //single();
  //updateScreen();
  //lc.setLed(0,random(8),random(8),HIGH);
  stars();
  delay(50);
}
uint8_t screen[8];

void updateScreen(void){
  // take the chip select low to select the device:

  for(int i = 0; i<8; i++){
    // take the chip select low to select the device:
    digitalWrite(chipSelectPin, LOW);

    SPI.transfer(i+1);  // Send row number
    SPI.transfer(0x55); // Send register location  

    // take the chip select high to de-select:
    digitalWrite(chipSelectPin, HIGH);
  }
}

void stars(void){
  static bool fill = true;
  static uint8_t data[8];

  bool led;
  int8_t counter = 0;
  uint8_t randomX;
  uint8_t randomY;
  
  for(uint8_t t = 0; t < 16; t++){
    
    randomX = random(8);
    randomY = random(8);

    led = (data[randomY] & 1 << randomX);

    if(false == led && true == fill ){
      //lc.setLed(0, randomX, randomY, HIGH);
      data[randomY] |= 1 << randomX;
      lc.setRow(0, randomY, data[randomY]);
      break;
    }

    if(true == led && false == fill ){
      data[randomY] ^= 1 << randomX;
      lc.setRow(0, randomY, data[randomY]);
      break;
    }
    
  }
  for(uint8_t y = 0; y < 8; y++){
      if(0xFF == data[y]){
        counter++;
      }
      if(0x00 == data[y]){
        counter--;
      }
  }

  if(counter > 7){
      fill = false;
  }
  if(counter < -7){
    fill = true;
  }
}
