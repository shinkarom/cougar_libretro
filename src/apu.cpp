#include "apu.h"

#include <cstring>
#include <cmath>

namespace apu {
	
	static uint16_t buffer[samplesPerTick*2];
	
	void init() {
		memset(buffer, 0, apu::samplesPerTick*2*2);
	}
	
	void deinit() {
		
	}
	
	uint16_t* process() {
		static unsigned phase;
	   for (unsigned i = 0; i < samplesPerTick; i++, phase++)
	   {
		  int16_t val = 0x800 * sinf(2.0f * M_PI * phase * 
			(audioSampleRate*1.0f/100) / (audioSampleRate *1.0f));
		  buffer[i * 2] = val;
		  buffer[i * 2 + 1] = val;
	   }

	   phase %= 100;
		
		return buffer;
	}
	
}