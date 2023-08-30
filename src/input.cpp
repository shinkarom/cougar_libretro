#include "input.h"

#include <iostream>

namespace input {
	
	uint32_t pressed = 0;
	uint32_t old = 0;
	uint32_t released = 0;
	
	void init() {
		
	}
	
	void deinit() {
		
	}
	
	uint32_t pressedButtons() {
		return pressed;
	}
	
	uint32_t releasedButtons() {
		return released;
	}

	bool isButtonPressed(int num) {
		if(num < 0 || num >= numButtons) {
			return false;
		} else {
			return pressed & (1 << (num + 1));
		}
	}
	
	bool isButtonReleased(int num) {
		if(num < 0 || num >= numButtons) {
			return false;
		} else {
			return released & (1 << (num + 1));
		}
	}

	void setPressedButtons(uint32_t value) {
		old = pressed;
		pressed = value;
		released = old & ~pressed;
	}
	
}