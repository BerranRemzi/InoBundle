#include "Brick.h"

bool checkBit(uint8_t _data, uint8_t _pos){
	return (_data >> _pos) & 1;
}

Brick::Brick() {
	reset();
}

void Brick::reset() {
	for (int i{ 0 }; i < 8; ++i)
		screen[i] = 0;

	brickHeight = SCREEN_HEIGHT - 1;
	screen[brickHeight] = BRICK_DEFAULT;

	totalTicks = TICK_MEDIUM;	// medium level 250ms
}


void Brick::update() {
	static uint8_t cycle = totalTicks;
	if (cycle < totalTicks) {
		cycle++;
		return;
	}
	cycle = 0;

	if(DIR_DOWN == KB_GetLastDirection()){
		CollisionDetection();
	}
	
	PlaceBrick();
	MoveBrick();

	render();
}

void Brick::CollisionDetection(){

}

void Brick::render() {
	for(uint8_t y = 0; y< 8;y++){
		for(uint8_t x = 0; x<8;x++){
			if(checkBit(screen[0],x)){
				AB_SetLed(7-x, 7-y, LED_ON);
			}else{
				AB_SetLed(7-x, 7-y, LED_OFF);
			}
		}
	}
}

bool  Brick::isOnScreen(){
	return (brickOnScreenLength > 0);
}

void Brick::MoveBrick() {

	// Delete last part of the brick
	#if 0
	if (direction == DIR_LEFT) {
		for (int i{ SCREEN_HEIGHT - 1 }; i > -1; --i)
			if (checkBit(screen[brickHeight], i)) {
				AB_SetLed(i, brickHeight, LED_OFF);
			}
			screen[brickHeight] <<= 1;
	}
	else if (direction == DIR_RIGHT) {
		for (int i{ 0 }; i < SCREEN_HEIGHT; ++i)
			if (checkBit(screen[brickHeight], i)) {
				AB_SetLed(SCREEN_HEIGHT - 1 - i, brickHeight, LED_OFF);
			}
			screen[brickHeight] >>= 1;
	}

	// Print brick
	for (int i{ 0 }; i < SCREEN_HEIGHT; ++i) {
		for (int j{ 0 }; j < SCREEN_HEIGHT; ++j) {
			if (checkBit(screen[i], j)) {
				AB_SetLed(SCREEN_HEIGHT - 1 - j, i, LED_ON);
			}
		}
	}
	#endif


	if(direction == DIR_RIGHT){
		screen[0] >>= 1;
		if(brickDefaultLenght - brickOnScreenLength > 0){
			screen[0] |= 0x80;
			brickOnScreenLength++;
		}
		if(screen[0] == 0x00){
			direction = DIR_LEFT;
			brickOnScreenLength = 0;
		}
	}else if(direction == DIR_LEFT){
		screen[0] <<= 1;
		if(brickDefaultLenght - brickOnScreenLength > 0){
			screen[0] |= 0x01;
			brickOnScreenLength++;
		}
		if(screen[0] == 0x00){
			direction = DIR_RIGHT;
			brickOnScreenLength = 0;
		}
	}
}

void Brick::PlaceBrick() {
	if (direction == DIR_DOWN) {
		++brickHeight;
		screen[brickHeight] = BRICK_DEFAULT;
	}
}
