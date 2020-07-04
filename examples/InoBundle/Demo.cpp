#include "Demo.h"


void Demo(void){
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
      AB_SetRow(randomY, data[randomY]);
      break;
    }

    if(true == led && false == fill ){
      data[randomY] ^= 1 << randomX;
      AB_SetRow(randomY, data[randomY]);
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