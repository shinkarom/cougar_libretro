#pragma once

#include <cstdint>
#include "common.h"

namespace apu {
	
	void init();
	void deinit();
	
	int16_t* process();
	
	int getVolume(int playerNum);
	void setVolume(int playerNum, int value);
	void setFrequency(int playerNum, float value);
	float getFrequency(int playerNum);
	void setWaveform(int playerNum, int value);
	void setNote(int playerNum, int value);
}