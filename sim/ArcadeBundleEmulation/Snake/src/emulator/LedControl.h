#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

class LedControl {
public:
	LedControl(int dataPin, int clkPin, int csPin, int numDevices) {
		showConsoleCursor(false);
	}
	LedControl(int csPin, int numDevices) {
		showConsoleCursor(false);
	}
	void setRow(int addr, int row, byte value) {
		printToScreen(row, value);
	}
	void shutdown(int addr, bool b) {

	}
	void setIntensity(int addr, int intensity) {

	}
	void clearDisplay(int addr) {

	}
	void setLed(int addr, int row, int column, bool state) {
		int offset;
		byte val = 0x00;

		//if (addr < 0 || addr >= maxDevices)
		//	return;
		//if (row < 0 || row>7 || column < 0 || column>7)
		//	return;
		offset = addr * 8;
		val = 0b1000'0000 >> column;
		if (state)
			status[offset + row] = status[offset + row] | val;
		else {
			val = ~val;
			status[offset + row] = status[offset + row] & val;
		}
		printToScreen(row, status[row]);
	}

private:
	/* The array for shifting the data to the devices */
	byte spidata[16];
	/* Send out a single command to the device */
	void printToScreen(int row, byte data) {
		setCursor(row, 0);
		for (int x{ 7 }; x >= 0; --x) {
			std::cout << ((data >> x & 1U) ? "#" : " ");
		}
	}

	void setCursor(short x, short y) {
		HANDLE h{ GetStdHandle(STD_OUTPUT_HANDLE) };
		COORD position{ y, x };
		SetConsoleCursorPosition(h, position);
	}

	void showConsoleCursor(bool showFlag) {
		HANDLE out{ GetStdHandle(STD_OUTPUT_HANDLE) };
		CONSOLE_CURSOR_INFO     cursorInfo;
		GetConsoleCursorInfo(out, &cursorInfo);
		cursorInfo.bVisible = showFlag;
		SetConsoleCursorInfo(out, &cursorInfo);
	}

	/* We keep track of the led-status for all 8 devices in this array */
	byte status[8];
	/* Data is shifted out of this pin*/
	int SPI_MOSI;
	/* The clock is signaled on this pin */
	int SPI_CLK;
	/* This one is driven LOW for chip selectzion */
	int SPI_CS;
	/* The maximum number of devices we use */
	int maxDevices;
};