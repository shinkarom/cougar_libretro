#include "apu.h"
#include "fs.h"
#include "audioplayer.h"

#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "stb_vorbis.h"

#include "audioplayer.h"

namespace apu {	

	constexpr int sqrs[4] = {100, 141, 173, 200};
	
	static uint16_t buffer[samplesPerTick*2];
	int pos;
	Player players[maxPlayers];
	
	short* output;
	bool playing = false;
	
	void init() {
		memset(buffer, 0, samplesPerTick*2*2);
	}
	
	void deinit() {
		
	}
	
	uint16_t* process() {
		memset(buffer, 0, samplesPerTick * 2);
		int64_t sample = 0;
		int playings = 0;
		if(!playing) {
			
		} else {
			for (int i = 0; i<maxPlayers; i++) {
				if(players[i].process()) {
					playings++;
				}
			}
			for(int s = 0; s<samplesPerTick*2; s++) {
				sample = 0;
				for (int i = 0; i<maxPlayers; i++) {
					if(players[i].isPlaying()) {
						sample += players[i].getBuffer()[s];
					}
				}
				sample = sample * 100 / sqrs[playings];
				buffer[s] = (int16_t)sample;
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
			players[playerNum].loadTrack(output, result	);
			std::cout<<"[COUGAR] "<<sample_rate<<" "<<result<<std::endl;
			playing = true;
		}
		
	}

	void setVolume(int playerNum, int value) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		players[playerNum].setVolume(value);
	}
	
	void play(int playerNum) { 
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		players[playerNum].start();
	}
	
	void stop(int playerNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		players[playerNum].stop();
	}
	
}