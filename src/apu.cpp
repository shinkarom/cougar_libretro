#include "apu.h"
#include "fs.h"

#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "stb_vorbis.h"

namespace apu {	
	
	static uint16_t buffer[samplesPerTick*2];
	int pos;
	
	short* output;
	bool playing = false;
	
	void init() {
		memset(buffer, 0, apu::samplesPerTick*2*2);
	}
	
	void deinit() {
		
	}
	
	uint16_t* process() {
		memset(buffer, 0, samplesPerTick * 2);
		if(!playing) {
			
		} else {
			for(int i = 0; i<samplesPerTick; i++) {
				buffer[i*2] = output[pos];
				buffer[i*2+1] = output[pos];
				pos++;
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
		int channels = 0;
		int sample_rate = 0;
		
		auto result = stb_vorbis_decode_memory((unsigned char*)fileBuffer, r, &channels, &sample_rate, &output);
		// TODO remove this line after implementing players.
		delete[] fileBuffer;
		if(result == -1) {
			std::cout<<"[COUGAR] couldn't decode "<<fileName<<std::endl;
		} else {
			std::cout<<"[COUGAR] "<<sample_rate<<std::endl;
			playing = true;
		}
		
	}
	
}