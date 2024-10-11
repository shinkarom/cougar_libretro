#pragma once

#include <cstdint>
#include "common.h"

namespace apu {
	
	void init();
	void deinit();
	
	int16_t* process();
	
	float getVolume(int playerNum);
	void setVolume(int playerNum, float value);
	void setFrequency(int playerNum, float value);
	float getFrequency(int playerNum);
	void setWaveform(int playerNum, int value);
	int getWaveform(int playerNum);
	void setNote(int playerNum, int value);
}