#include "apu.h"
#include "fs.h"
#include "audioplayer.h"

#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "audioplayer.h"

namespace apu {	
	
	static int16_t buffer[samplesPerTick*2];
	int pos;
	Player players[maxPlayers];
	
	short* output;
	bool playing = false;
	
	void init() {
		memset(buffer, 0, samplesPerTick*sizeof(int16_t)*2);
	}
	
	void deinit() {
		
	}
	
	int16_t* process() {
		memset(buffer, 0, samplesPerTick * sizeof(int16_t) * 2);
		float sample = 0;
		for(int s = 0; s<samplesPerTick; s++) {
			sample = 0;
			for (int i = 0; i<maxPlayers; i++) {
				float cursample = players[i].tick();
				//std::cout<<"<"<<cursample<<" "<<players[i].getVolume()<<std::endl;
				cursample = cursample * players[i].getVolume();
				sample += cursample;
			}
			if(sample>1.0){
				sample = 1.0;
			} else if (sample < -1.0) {
				sample = -1.0;
			}
			//std::cout<<sample<<std::endl;
			uint16_t finalsample = (int16_t)(std::round(sample*INT16_MAX));
			buffer[s*2] = finalsample;
			buffer[s*2+1] = finalsample;
			std::cout<<(int16_t)(finalsample)<<">"<<std::endl;
		}
		//std::cout<<"---"<<std::endl;		
		return buffer;
	}
	

	int getVolume(int playerNum) {
		if(playerNum<0 || playerNum>=maxPlayers) {
			return 0;
		}
		return players[playerNum].getVolume();
	}

	void setVolume(int playerNum, int value) {
		if(playerNum<0 || playerNum>=maxPlayers || value < 0 || value>maxVolume) {
			return;
		}
		players[playerNum].setVolume(value*1.0/maxVolume);
		std::cout<<players[playerNum].getVolume()<<std::endl;
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
	
	void setNote(int playerNum, int value) {
		if(playerNum<0 || playerNum>=maxPlayers || value >= 127 || value < 0) {
			return;
		}
		
		players[playerNum].setFrequency(440.0 * std::pow(2.0, (value - 69) / 12.0));
	}
	
}