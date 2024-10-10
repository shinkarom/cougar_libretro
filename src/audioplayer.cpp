#include "audioplayer.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cmath>

Player::Player() {
	playing = false;
	volume = 1.0;
	frequency = 0;
	phase = 0;
	waveform = SINE;
}

Player::~Player() {
	
}

float Player::tick() {
	if (!playing || !frequency) {
        return 0; // If not playing, simply return
    }
	float sample;
	switch(waveform) {
		case SINE:
			sample = playSine();
			break;
		case SQUARE:
			sample = playSquare();
			break;
		case SAWTOOTH:
			sample = playSawtooth();
			break;
		case TRIANGLE:
			sample = playTriangle();
			break;
		default:
			sample = 0;
			break;
	}
	updatePhase();
	return sample;
}

float Player::playSine() {
	float sample = std::sin(phase);	
	return sample;
}

float Player::playSquare() {
    float sample = (std::sin(phase) >= 0) ? 1.0 : -1.0;
    return sample;
}

float Player::playSawtooth() {
    double t = phase / (2.0 * M_PI);
    float sample = (2.0 * t - 1.0);
    return sample;
}

float Player::playTriangle() {
    double t = phase / (2.0 * M_PI);
    float sample = (t < 0.5 ? 4 * t - 1.0 : 3.0 - 4 * t);	
    return sample;
}

void Player::updatePhase() {
	phase += (2.0 * M_PI * frequency) / audioSampleRate;

    if (phase >= 2.0 * M_PI) {
        phase -= 2.0 * M_PI;
    }
}

bool Player::isPlaying() {
	return playing;
}

float Player::getVolume() {
	return volume;
}

void Player::setVolume(float value) {
	if(value <0 || value > 1.00) {
		return;
	}
	volume = value;
}

void Player::setFrequency(float value) {
	frequency = value;
	playing = (frequency!=0);
}

float Player::getFrequency() {
	return frequency;
}

void Player::setWaveform(WaveformType value) {
	if(value != numWaveforms){
		waveform = value;
		phase = 0;
	}
}