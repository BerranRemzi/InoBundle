#include "ArcadeBundle_HAL.h"

const uint8_t row[] =  {4, 3, 8, 12, A4, 9, A3, 5};
const uint8_t col[] =  {2, A2, 13, 6, 10, 11, A5, 7};

extern uint8_t AB_screen[8][8];

void AB_HAL_Init(void){
  for (uint8_t i = 0; i < SCREEN_WIDTH; i++) {
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }
  WriteRows(ROW_OFF);
}

void AB_HAL_UpdateScreen(void) {
  static uint8_t y = 0;
  digitalWrite(row[y], ROW_OFF);

  y++;
  if (y > (SCREEN_HEIGHT-1)) {
    y = 0;
  }

  for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
    digitalWrite(col[x], AB_screen[y][x]);
  }
  digitalWrite(row[y], ROW_ON);
}

void WriteRows(uint8_t _state) {
  for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
    digitalWrite(row[y], _state);
  }
}
void WriteCols(uint8_t _state) {
  for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
    digitalWrite(col[x], _state);
  }
}

void AB_HAL_AnalogPrescaler(uint8_t _prescaler){
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // clear prescaler bits
  
  switch(_prescaler){
    case 2: ADCSRA |= bit (ADPS0);                break;                //   2
    case 4: ADCSRA |= bit (ADPS1);                break;                //   4  
    case 8: ADCSRA |= bit (ADPS0) | bit (ADPS1);  break;                //   8  
    case 16: ADCSRA |= bit (ADPS2);               break;                //  16 
    case 32: ADCSRA |= bit (ADPS0) | bit (ADPS2); break;                //  32 
    case 64: ADCSRA |= bit (ADPS1) | bit (ADPS2); break;                //  64 
    default: ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2); break; // 128
  }
}