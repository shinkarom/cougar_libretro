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
		double tick();
		bool isPlaying();
		float getVolume();
		void setVolume(float value);
		void setFrequency(float value);
		float getFrequency();
		void setWaveform(WaveformType value);
		WaveformType getWaveform();
		
	private:
		double playSine();
		double playSquare();
		double playSawtooth();
		double playTriangle();
		double playNoise();
		void updatePhase();
		float volume;
		bool playing;
		float frequency;
		double phase;
		WaveformType waveform;
};