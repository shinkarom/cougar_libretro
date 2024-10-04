#pragma once

#include <cstdint>
#include "common.h"

namespace apu {
	
	void init();
	void deinit();
	
	uint16_t* process();
	
	int getVolume(int playerNum);
	void setVolume(int playerNum, int value);
	void setFrequency(int playerNum, float value);
	float getFrequency(int playerNum);
}