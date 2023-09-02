#pragma once

#include <cstdint>

#include "common.h"

class Player {
	public:

		Player();
		~Player();
		void loadTrack(int16_t* tr);
		int16_t* process();
		void seek(int position);
		int tell();
		bool isActive();
		bool isPlaying();
		int16_t* getBuffer();
		void setVolume(int value);
		
	private:
		int pos;
		int volume;
		int16_t* track;
		int16_t buffer[samplesPerTick*2];
		bool active;
		bool playing;
};