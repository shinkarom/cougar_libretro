#pragma once

#include <cstdint>
#include "common.h"

namespace apu {
	
	void init();
	void deinit();
	
	int16_t* process();
	
	void writeReg(uint16_t reg, uint8_t value);
}