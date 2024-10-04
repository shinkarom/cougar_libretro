#include "apu.h"
#include "fs.h"
#include "audioplayer.h"

#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "audioplayer.h"

namespace apu {	

	constexpr int sqrs[4] = {100, 141, 173, 200};
	
	static uint16_t buffer[samplesPerTick*2];
	int pos;
	Player players[maxPlayers];
	
	short* output;
	bool playing = false;
	
	void init() {
		memset(buffer, 0, samplesPerTick*sizeof(int16_t)*2);
	}
	
	void deinit() {
		
	}
	
	uint16_t* process() {
		memset(buffer, 0, samplesPerTick * 2);
		int64_t sample = 0;
		int playings = 0;
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
				
		return buffer;
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
	
	void setFrequency(int playerNum, float value) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return;
		}
		players[playerNum].setFrequency(value);
	}
	
	float getFrequency(int playerNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return false;
		}
		return players[playerNum].getFrequency();
	}
	
	void setWaveform(int playerNum, int value) {
		if(playerNum<0 || playerNum>=maxPlayers || value >= numWaveforms || value < 0) {
			return;
		}
		players[playerNum].setWaveform((WaveformType)value);
	}
	
}