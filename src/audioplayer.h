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
		float tick();
		bool isPlaying();
		float getVolume();
		void setVolume(float value);
		void setFrequency(float value);
		float getFrequency();
		void setWaveform(WaveformType value);
		
	private:
		float playSine();
		float playSquare();
		float playSawtooth();
		float playTriangle();
		void updatePhase();
		float volume;
		bool playing;
		float frequency;
		double phase;
		WaveformType waveform;
};