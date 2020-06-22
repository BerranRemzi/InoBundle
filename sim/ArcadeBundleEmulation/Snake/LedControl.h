#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

class LedControl {
public:
    LedControl(int dataPin, int clkPin, int csPin, int numDevices) {

    }
    void setRow(int addr, int row, byte value) {

    }
    void shutdown(int addr, bool b) {

    }
    void setIntensity(int addr, int intensity) {

    }
    void clearDisplay(int addr) {

    }
    void setLed(int addr, int row, int column, boolean state) {
        int offset;
        byte val = 0x00;

        if (addr < 0 || addr >= maxDevices)
            return;
        if (row < 0 || row>7 || column < 0 || column>7)
            return;
        offset = addr * 8;
        val = 0x10000000 >> column;
        if (state)
            status[offset + row] = status[offset + row] | val;
        else {
            val = ~val;
            status[offset + row] = status[offset + row] & val;
        }
        spiTransfer(addr, row + 1, status[offset + row]);
    }

private:
    /* The array for shifting the data to the devices */
    byte spidata[16];
    /* Send out a single command to the device */
    void spiTransfer(int addr, byte opcode, byte data) {

    }

    /* We keep track of the led-status for all 8 devices in this array */
    byte status[64];
    /* Data is shifted out of this pin*/
    int SPI_MOSI;
    /* The clock is signaled on this pin */
    int SPI_CLK;
    /* This one is driven LOW for chip selectzion */
    int SPI_CS;
    /* The maximum number of devices we use */
    int maxDevices;
};