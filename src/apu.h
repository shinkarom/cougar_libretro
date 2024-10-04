#pragma once

#include <cstdint>
#include "common.h"

namespace apu {
	
	void init();
	void deinit();
	
	uint16_t* process();
	
	void loadFile(int playerNum, int trackNum);
	int getVolume(int playerNum);
	void setVolume(int playerNum, int value);
	bool isPlaying(int playerNum);
	void setPlaying(int playerNum, bool value);
	int32_t getPosition(int playerNum);
	void setPosition(int playerNum, int32_t value);
	int32_t trackLength(int playerNum);
	bool isLooping(int playerNum);
	void setLooping(int playerNum, bool value);
	void setFrequency(int playerNum, float value);
	float getFrequency(int playerNum);
}