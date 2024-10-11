#include "apu.h"
#include "fs.h"

#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "opl3.h"

namespace apu {	
	
	static int16_t buffer[samplesPerTick*2];
	opl3_chip chip;
	
	short* output;
	bool playing = false;
	
	void init() {
		memset(buffer, 0, samplesPerTick*sizeof(int16_t)*2);
		OPL3_Reset(&chip, audioSampleRate);
		OPL3_WriteRegBuffered(&chip, 0x105, 0x01);
	}
	
	void deinit() {
		
	}
	
	int16_t* process() {
		memset(buffer, 0, samplesPerTick * sizeof(int16_t) * 2);
		OPL3_GenerateStream(&chip, buffer,samplesPerTick);	
		return buffer;
	}
	
	void writeReg(uint16_t reg, uint8_t value) {
		OPL3_WriteRegBuffered(&chip, reg, value);
	}
	
}