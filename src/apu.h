#pragma once

#include <cstdint>
#include "common.h"

namespace apu {
	
	void init();
	void deinit();
	
	uint16_t* process();
	
	void loadFile(int playerNum, const char* fileName);
	void setVolume(int playerNum, int value);
	void play(int playerNum);
	void stop(int playerNum);
	int32_t tell(int playerNum);
	void seek(int playerNum, int32_t pos);
	int32_t trackLength(int playerNum);
}