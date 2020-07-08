#include "ArcadeBundle_HAL.h"

extern uint8_t AB_screen[8][8];

void AB_HAL_InitPins(void){
    // start the SPI library:
    SPI.begin();
    pinMode(SPI_CS_PIN, OUTPUT);
    digitalWrite(SPI_CS_PIN, HIGH);
}

void AB_HAL_Init(void){
    AB_HAL_InitPins();
    AB_HAL_DisplayTest();
    AB_HAL_SetScanLimit(7);
    AB_HAL_DecodeMode();
    AB_HAL_Shutdown(true);
    AB_HAL_Shutdown(false);
    AB_HAL_SetIntensity(1);
    AB_HAL_ClearDisplay();
}

void AB_HAL_SpiTransfer(volatile uint8_t opcode, volatile uint8_t data) {
    //enable the line 
    digitalWrite(SPI_CS_PIN,LOW);

    SPI.transfer(opcode);
    SPI.transfer(data);
    //latch the data onto the display
    digitalWrite(SPI_CS_PIN,HIGH);
}

void AB_HAL_ClearDisplay(void) {
    for(int i=0;i<8;i++) {
        AB_HAL_SpiTransfer(OP_DIGIT0+i,LED_OFF);
    }
}

void AB_HAL_Shutdown(bool b) {
    AB_HAL_SpiTransfer(OP_SHUTDOWN, !b);
}

void AB_HAL_UpdateScreen(void){
  uint8_t data = 0x00;
  for(uint8_t y = 0; y<8;y++){

	data = 0x00;
	for(uint8_t x = 0; x < 8; x++){
		if(AB_screen[x][y] > 0){
			data |= 1 << x;
		}
	}
    AB_HAL_SpiTransfer(OP_DIGIT0 + y, data);
  }
}

void AB_HAL_DecodeMode(void){
    //decode is done in source
    AB_HAL_SpiTransfer(OP_DECODEMODE, 0);
}

void AB_HAL_DisplayTest(void){
    AB_HAL_SpiTransfer(OP_DISPLAYTEST, 0);
}

void AB_HAL_SetScanLimit(uint8_t _limit){
    if((_limit >= 0) && (_limit < 8)){
        AB_HAL_SpiTransfer(OP_SCANLIMIT, _limit);
    }
}

void  AB_HAL_SetIntensity( uint8_t intensity) {
    if(intensity>=0 && intensity<16){
        AB_HAL_SpiTransfer(OP_INTENSITY, intensity);
    }

}