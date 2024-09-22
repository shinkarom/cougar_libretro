#pragma once

#include <cstdint>

#include "common.h"

class Player {
	public:

		Player();
		~Player();
		void loadTrack(int16_t* tr, int length);
		void unloadTrack();
		bool process();
		void setPosition(int value);
		int64_t getPosition();
		bool isActive();
		bool isPlaying();
		void setPlaying(bool value);
		int16_t* getBuffer();
		int getVolume();
		void setVolume(int value);
		int64_t getLength();
		bool isLooping();
		void setLooping(bool value);
		
	private:
		int pos;
		int volume;
		bool looping;
		int16_t* track;
		int64_t trackLength;
		int16_t buffer[samplesPerTick*2];
		bool active;
		bool playing;
};