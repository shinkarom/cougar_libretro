#include "apu.h"
#include "fs.h"

#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "stb_vorbis.h"

namespace apu {
	
	static uint16_t buffer[samplesPerTick*2];
	
	short* output;
	bool playing = false;
	
	void init() {
		memset(buffer, 0, apu::samplesPerTick*2*2);
	}
	
	void deinit() {
		
	}
	
	uint16_t* process() {
		static unsigned phase;
	   for (unsigned i = 0; i < samplesPerTick; i++)
	   {
		if(playing) {
			buffer[i * 2] = output[phase];
			buffer[i * 2 + 1] = output[phase];
			phase++;
		} else {
			buffer[i * 2] = 0;
			buffer[i * 2 + 1] = 0;
		}
		  
	   }
		
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
		
		int error;
		auto alloc_buffer = new stb_vorbis_alloc;
		int channels = 0;
		int sample_rate = 0;
		auto n = stb_vorbis_decode_memory((unsigned char*)fileBuffer, r,
                                  &channels, &sample_rate, &output);
		if(channels > 1 || sample_rate != audioSampleRate) {
			std::cout<<"[COUGAR] Can't play "<<fileName<<", must be mono with sample rate "<<audioSampleRate<<"."<<std::endl;
		}
		playing = true;
		// TODO remove this line after implementing players.
		delete[] fileBuffer;
	}
	
}