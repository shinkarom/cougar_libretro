#pragma once

#include <cstdint>
#include "common.h"

namespace apu {
	
	void init();
	void deinit();
	
	uint16_t* process();
	
	void playFile(int playerNum, const char* fileName);
	void setVolume(int playerNum, int value);
}