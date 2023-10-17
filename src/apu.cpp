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
	int audioBits = maxAudioBits;
	
	void init() {
		memset(buffer, 0, samplesPerTick*2*2);
	}
	
	void deinit() {
		
	}
	
	void setBits(int bits) {
		if(bits > 0 && bits <= maxAudioBits) {
			audioBits = bits;
		}
	}
	
	uint16_t* process() {
		memset(buffer, 0, samplesPerTick * 2);
		int64_t sample = 0;
		int playings = 0;
		for (int i = 0; i<maxPlayers; i++) {
			if(players[i].process(audioBits)) {
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
				
		return buffer;
	}
	
	void loadFile(int playerNum, int trackNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		if(trackNum < 0 || trackNum >= maxAudioTracks) {
			return;
		}
		char fullFilename[256];
		sprintf(fullFilename, "/SND/SND_%04X.ogg", trackNum);
		
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
		
		delete[] fileBuffer;
		if(result == -1) {
			std::cout<<"[COUGAR] couldn't decode "<<fullFilename<<std::endl;
		} else {
			players[playerNum].loadTrack(output, result);
			std::cout<<"[COUGAR] "<<sample_rate<<" "<<result<<std::endl;
		}
		
	}

	int getVolume(int playerNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return 0;
		}
		return players[playerNum].getVolume();
	}

	void setVolume(int playerNum, int value) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		players[playerNum].setVolume(value);
	}
	
	int32_t getPosition(int playerNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return -1;
		}
		return players[playerNum].getPosition();
	}
	
	void setPosition(int playerNum, int32_t value) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		players[playerNum].setPosition(value);
	}
	
	int32_t trackLength(int playerNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return -1;
		}
		return players[playerNum].getLength();
	}
	
	bool isLooping(int playerNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return -1;
		}
		return players[playerNum].isLooping();
	}
	
	void setLooping(int playerNum, bool value) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		players[playerNum].setLooping(value);
	}
	
	bool isPlaying(int playerNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return false;
		}
		return players[playerNum].isPlaying();
	}
	
	void setPlaying(int playerNum, bool value) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		players[playerNum].setPlaying(value);
	}
	
}