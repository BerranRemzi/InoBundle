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
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

LedControl::LedControl(int dataPin, int clkPin, int csPin, int numDevices) {
    SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    if(numDevices<=0 || numDevices>8 )
        numDevices=8;
    maxDevices=numDevices;
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
    SPI_MOSI=dataPin;
    for(int i=0;i<64;i++) 
        status[i]=0x00;
    for(int i=0;i<maxDevices;i++) {
        spiTransfer(i,OP_DISPLAYTEST,0);
        //scanlimit is set to max on startup
        setScanLimit(i,7);
        //decode is done in source
        spiTransfer(i,OP_DECODEMODE,0);
        clearDisplay(i);
        //we go into shutdown-mode on startup
        shutdown(i,true);
    }
}

LedControl::LedControl(int csPin, int numDevices) {
    SPI_CS=csPin;
    if(numDevices<=0 || numDevices>8 )
        numDevices=8;
    maxDevices=numDevices;

    // start the SPI library:
    SPI.begin();
    softSPI = false;

    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
    
    for(int i=0;i<64;i++) 
        status[i]=0x00;
    for(int i=0;i<maxDevices;i++) {
        spiTransfer(i,OP_DISPLAYTEST,0);
        //scanlimit is set to max on startup
        setScanLimit(i,7);
        //decode is done in source
        spiTransfer(i,OP_DECODEMODE,0);
        clearDisplay(i);
        //we go into shutdown-mode on startup
        shutdown(i,true);
    }
}

int LedControl::getDeviceCount() {
    return maxDevices;
}

void LedControl::shutdown(int addr, bool b) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(b)
        spiTransfer(addr, OP_SHUTDOWN,0);
    else
        spiTransfer(addr, OP_SHUTDOWN,1);
}

void LedControl::setScanLimit(int addr, int limit) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(limit>=0 && limit<8)
        spiTransfer(addr, OP_SCANLIMIT,limit);
}

void LedControl::setIntensity(int addr, int intensity) {
    if(addr<0 || addr>=maxDevices)
        return;
    if(intensity>=0 && intensity<16)	
        spiTransfer(addr, OP_INTENSITY,intensity);
}

void LedControl::clearDisplay(int addr) {
    int offset;

    if(addr<0 || addr>=maxDevices)
        return;
    offset=addr*8;
    for(int i=0;i<8;i++) {
        status[offset+i]=0;
        spiTransfer(addr, i+1,status[offset+i]);
    }
}

void LedControl::setLed(int addr, int row, int column, boolean state) {
    int offset;
    byte val=0x00;

    if(addr<0 || addr>=maxDevices)
        return;
    if(row<0 || row>7 || column<0 || column>7)
        return;
    offset=addr*8;
    val=B10000000 >> column;
    if(state)
        status[offset+row]=status[offset+row]|val;
    else {
        val=~val;
        status[offset+row]=status[offset+row]&val;
    }
    spiTransfer(addr, row+1,status[offset+row]);
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

void LedControl::spiTransfer(int addr, volatile byte opcode, volatile byte data) {
    //Create an array with the data to shift out
    int offset=addr*2;
    int maxbytes=maxDevices*2;

    for(int i=0;i<maxbytes;i++)
        spidata[i]=(byte)0;
    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;
    //enable the line 
    digitalWrite(SPI_CS,LOW);
    if(true == softSPI){
        //Now shift out the data 
        for(int i=maxbytes;i>0;i--){
            shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
        }
    }else{
        for(int i=maxbytes;i>0;i--){
            //shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
            SPI.transfer(spidata[i-1]);
        }
    }
    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
}    

void LedControl::updateScreen(uint8_t _input[][8], uint8_t _size){

  uint8_t data = 0x00;
  for(uint8_t y = 0; y<8;y++){

	data = 0x00;
	for(uint8_t x = 0; x < 8; x++){
		if(_input[x][y] > 0){
			data |= 1 << x;
		}
	}
	    // take the chip select low to select the device:
    digitalWrite(SPI_CS, LOW);

    SPI.transfer(y+1);  // Send row number
    SPI.transfer(data); // Send register location  

    // take the chip select high to de-select:
    digitalWrite(SPI_CS, HIGH);
  }
  
  /*
  for(int i = 0; i<8; i++){
    // take the chip select low to select the device:
    digitalWrite(chipSelectPin, LOW);

    SPI.transfer(i+1);  // Send row number
    SPI.transfer(0x55); // Send register location  

    // take the chip select high to de-select:
    digitalWrite(chipSelectPin, HIGH);
  }*/
}

uint8_t screen[8][8];


void AB_InitPins(void){

    // start the SPI library:
    SPI.begin();
    pinMode(SPI_CS_PIN, OUTPUT);
    digitalWrite(SPI_CS_PIN, HIGH);


}

void AB_InitScreen(void){
    //spiTransfer(addr, OP_SHUTDOWN,1);
    digitalWrite(SPI_CS_PIN,LOW);
    SPI.transfer(OP_SHUTDOWN);
    SPI.transfer(1);
    digitalWrite(SPI_CS_PIN,HIGH);
    delay(1);

    //spiTransfer(addr, OP_INTENSITY,intensity);
    digitalWrite(SPI_CS_PIN,LOW);
    SPI.transfer(OP_INTENSITY);
    SPI.transfer(1);
    digitalWrite(SPI_CS_PIN,HIGH);
    delay(1);

    //lc.clearDisplay(0);
    for(int i=0;i<8;i++) {
        digitalWrite(SPI_CS_PIN,LOW);
        SPI.transfer(i);
        SPI.transfer(0x00);
        digitalWrite(SPI_CS_PIN,HIGH);
        delay(1);
    }
    
}

void AB_Setup(void){
    randomSeed(analogRead(0));
    KB_Setup();
    AB_InitPins();
    AB_InitScreen();
}

void AB_SetLed(uint8_t x, uint8_t y, uint8_t brightness) {
	screen[y][x] = brightness;
}

uint8_t AB_GetLed(uint8_t x, uint8_t y){
	return screen[y][x];
}

void AB_UpdateScreen(){
  uint8_t data = 0x00;
  for(uint8_t y = 0; y<8;y++){

	data = 0x00;
	for(uint8_t x = 0; x < 8; x++){
		if(screen[x][y] > 0){
			data |= 1 << x;
		}
	}
	    // take the chip select low to select the device:
    digitalWrite(SPI_CS_PIN, LOW);

    SPI.transfer(y+1);  // Send row number
    SPI.transfer(data); // Send register location  

    // take the chip select high to de-select:
    digitalWrite(SPI_CS_PIN, HIGH);
  }
}