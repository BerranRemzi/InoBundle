/*
 *    LedControl.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */


#include "ArcadeBundle.h"

//the opcodes for the MAX7221 and MAX7219
enum OP_CODE{
    OP_NOOP, 
    OP_DIGIT0, 
    OP_DIGIT1, 
    OP_DIGIT2, 
    OP_DIGIT3, 
    OP_DIGIT4,
    OP_DIGIT5,
    OP_DIGIT6, 
    OP_DIGIT7, 
    OP_DECODEMODE, 
    OP_INTENSITY,   
    OP_SCANLIMIT,  
    OP_SHUTDOWN,    
    OP_DISPLAYTEST = 15
};

LedControl::LedControl(int csPin, int numDevices) {
   
}

void LedControl::setScanLimit(int addr, int limit) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(limit>=0 && limit<8)
        spiTransfer(addr, OP_SCANLIMIT,limit);
}

void LedControl::setRow(int addr, int row, byte value) {
    int offset;
    if(addr<0 || addr>=maxDevices)
        return;
    if(row<0 || row>7)
        return;
    offset=addr*8;
    status[offset+row]=value;
    spiTransfer(addr, row+1,status[offset+row]);
}

void LedControl::setColumn(int addr, int col, byte value) {
    byte val;

    if(addr<0 || addr>=maxDevices)
        return;
    if(col<0 || col>7) 
        return;
    for(int row=0;row<8;row++) {
        val=value >> (7-row);
        val=val & 0x01;
        setLed(addr,row,col,val);
    }
}

void LedControl::setDigit(int addr, int digit, byte value, boolean dp) {
    int offset;
    byte v;

    if(addr<0 || addr>=maxDevices)
        return;
    if(digit<0 || digit>7 || value>15)
        return;
    offset=addr*8;
    v=pgm_read_byte_near(charTable + value); 
    if(dp)
        v|=B10000000;
    status[offset+digit]=v;
    spiTransfer(addr, digit+1,v);
}

void LedControl::setChar(int addr, int digit, char value, boolean dp) {
    int offset;
    byte index,v;

    if(addr<0 || addr>=maxDevices)
        return;
    if(digit<0 || digit>7)
        return;
    offset=addr*8;
    index=(byte)value;
    if(index >127) {
        //no defined beyond index 127, so we use the space char
        index=32;
    }
    v=pgm_read_byte_near(charTable + index); 
    if(dp)
        v|=B10000000;
    status[offset+digit]=v;
    spiTransfer(addr, digit+1,v);
}

uint8_t AB_screen[8][8];

void AB_InitPins(void){

    // start the SPI library:
    SPI.begin();
    pinMode(SPI_CS_PIN, OUTPUT);
    digitalWrite(SPI_CS_PIN, HIGH);
}

void AB_InitScreen(void){
    AB_DisplayTest();
    AB_SetScanLimit(7);
    AB_DecodeMode();
    AB_Shutdown(true);
    AB_Shutdown(false);
    AB_SetIntensity(1);
    AB_ClearDisplay();
}

void AB_Setup(void){
    randomSeed(analogRead(0));
    KB_Setup();
    AB_InitPins();
    AB_InitScreen();
}

void AB_SetRow(uint8_t y, uint8_t value) {
    for(int x=0; x < 8; x++){
        if((value >> x) & 1U){
            AB_screen[y][x] = LED_ON;
        }else{
            AB_screen[y][x] = LED_OFF;
        }
    }
}

void AB_SetLed(uint8_t x, uint8_t y, uint8_t brightness) {
	AB_screen[y][x] = brightness;
}

uint8_t AB_GetLed(uint8_t x, uint8_t y){
	return AB_screen[y][x];
}

void AB_DisplayTest(void){
    AB_SpiTransfer(OP_DISPLAYTEST, 0);
}

void AB_SetScanLimit(uint8_t _limit){
    if((_limit >= 0) && (_limit < 8)){
        AB_SpiTransfer(OP_SCANLIMIT, _limit);
    }
}

void AB_DecodeMode(void){
    //decode is done in source
    AB_SpiTransfer(OP_DECODEMODE, 0);
}

void AB_UpdateScreen(){
  uint8_t data = 0x00;
  for(uint8_t y = 0; y<8;y++){

	data = 0x00;
	for(uint8_t x = 0; x < 8; x++){
		if(AB_screen[x][y] > 0){
			data |= 1 << x;
		}
	}
    AB_SpiTransfer(OP_DIGIT0 + y, data);
  }
}

void AB_Shutdown(bool b) {
    AB_SpiTransfer(OP_SHUTDOWN, !b);
}

void  AB_SetIntensity( uint8_t intensity) {
    if(intensity>=0 && intensity<16){
        AB_SpiTransfer(OP_INTENSITY, intensity);
    }

}

void AB_ClearDisplay() {
    for(int i=0;i<8;i++) {
        AB_SpiTransfer(OP_DIGIT0+i,LED_OFF);
    }

    for(uint8_t y = 0; y<8;y++){
        for(uint8_t x = 0; x < 8; x++){
           AB_screen[x][y] = LED_OFF;
        }
    }
}

void AB_SpiTransfer(volatile byte opcode, volatile byte data) {
    //enable the line 
    digitalWrite(SPI_CS_PIN,LOW);

    SPI.transfer(opcode);
    SPI.transfer(data);
    //latch the data onto the display
    digitalWrite(SPI_CS_PIN,HIGH);
}    

bool AB_ClearAnimation(void){
    bool isScreenEmpty = true;

    for(int8_t x = SCREEN_WIDTH-1; x >=0; x--){
        for(int8_t y = 0; y<SCREEN_HEIGHT;y++){    
            if(AB_screen[x][y] == LED_ON){
                
                if(x < (SCREEN_WIDTH-1)){
                    AB_screen[x+1][y] = AB_screen[x][y]; 
                }
                AB_screen[x][y] = LED_OFF;
                isScreenEmpty = false;
                return isScreenEmpty;
            }
        }
    }

    return isScreenEmpty;
}