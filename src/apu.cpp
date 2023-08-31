#include "apu.h"
#include "fs.h"

#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>

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
	
	void playFile(int playerNum, const char* fileName) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		char fullFilename[256];
		sprintf(fullFilename, "/SND/%s", fileName);
		
		char* fileBuffer;
		auto r = fs::readBinaryFile(fullFilename, &fileBuffer, true);
		if(r==-1) {
			std::cout<<"[COUGAR] Couldn't read file "<<fullFilename<<std::endl;
			return;
		}
		// TODO remove this line after implementing players.
		delete[] fileBuffer;
	}
	
}