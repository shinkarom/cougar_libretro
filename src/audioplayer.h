#pragma once

#include <cstdint>

#include "common.h"

enum WaveformType {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH,
	numWaveforms
};

class Player {
	public:

		Player();
		~Player();
		bool process();
		bool isPlaying();
		int16_t* getBuffer();
		int getVolume();
		void setVolume(int value);
		void setFrequency(float value);
		float getFrequency();
		void setWaveform(WaveformType value);
		
	private:
		int16_t playSine();
		int16_t playSquare();
		int16_t playSawtooth();
		int16_t playTriangle();
		int volume;
		int16_t buffer[samplesPerTick*2];
		bool playing;
		float frequency;
		double phase;
		WaveformType waveform;
};