#pragma once

#include <cstdint>
#include "common.h"

namespace apu {
	
	void init();
	void deinit();
	
	uint16_t* process();
	
	void loadFile(int playerNum, const char* fileName);
	int getVolume(int playerNum);
	void setVolume(int playerNum, int value);
	bool isPlaying(int playerNum);
	void setPlaying(int playerNum, bool value);
	int32_t getPosition(int playerNum);
	void setPosition(int playerNum, int32_t value);
	int32_t trackLength(int playerNum);
	int64_t getLoopStart(int playerNum);
	int64_t getLoopEnd(int playerNum);
	void setLoopStart(int playerNum, int64_t value);
	void setLoopEnd(int playerNum, int64_t value);
	bool isLooping(int playerNum);
	void setLooping(int playerNum, bool value);
	int getPan(int playerNum);
	void setPan(int playerNum, int value);
}