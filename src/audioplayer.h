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
		void seek(int position);
		int tell();
		bool isActive();
		bool isPlaying();
		int16_t* getBuffer();
		void setVolume(int value);
		int64_t getLength();
		int64_t getLoopStart();
		int64_t getLoopEnd();
		void setLoopStart(int64_t value);
		void setLoopEnd(int64_t value);
		bool getLooping();
		void setLooping(bool value);
		void start();
		void stop();
		
	private:
		int pos;
		int volume;
		bool looping;
		int64_t loopStart;
		int64_t loopEnd;
		int16_t* track;
		int64_t trackLength;
		int16_t buffer[samplesPerTick*2];
		bool active;
		bool playing;
};