#include "audioplayer.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "stb_vorbis.h"

float previousSample;

Player::Player() {
	active = false;
	playing = false;
	volume = 10;
	previousSample = 0.0;
}

Player::~Player() {
	
}

void Player::loadTrack(int16_t* tr, int length) {
	track = tr;
	trackLength = length;
	active = true;
	playing = false;
	looping = false;
	pos = 0;
}

bool Player::process() {
	memset(buffer, 0, samplesPerTick * 2);
	if(!playing) {
		return false;
	} else {
		int bitDepth = 8;
		float floatScale = 1.0 / 32768.0;
		float quantizationStep = 1.0 / ((2 << (bitDepth - 1)) - 1);
		
		float cutoffFrequency = 14000.0;  // Adjust this to control the filter's cutoff frequency
		float lowPassAlpha = 2.0 * sin(M_PI * cutoffFrequency / audioSampleRate);

		
		auto remaining = trackLength - pos;
		auto samplesNow = remaining < samplesPerTick ? remaining : samplesPerTick;
		for(int i = 0; i<samplesNow; i++) {			
			float inputSample = track[pos] * floatScale;
			auto quantizedSample = round(inputSample / quantizationStep) * quantizationStep;
			auto lowPassedSample = (inputSample + previousSample * lowPassAlpha) / (1.0 + lowPassAlpha);
			auto volumedSample = lowPassedSample / 10.0 * volume;
			auto finalSample = volumedSample;
			previousSample = finalSample;
			buffer[i * 2] = static_cast<int16_t>(finalSample / floatScale);
			buffer[i*2+1] = buffer[i*2];
			pos++;
		}
		if(samplesNow < samplesPerTick) {
			if(!looping) {
				playing = false;
				pos = 0;
			}
		}
	}
	return true;
}

void Player::setPosition(int value) {
	if(value >=0 && value <= trackLength) {
		pos = value;
	}
}

int64_t Player::getPosition() {
	return pos;
}

bool Player::isActive() {
	return active;
}

bool Player::isPlaying() {
	return playing;
}

int16_t* Player::getBuffer() {
	return buffer;
}

int Player::getVolume() {
	return volume;
}

void Player::setVolume(int value) {
	if(value <0 || value > 10) {
		return;
	}
	volume = value;
}

void Player::unloadTrack() {
	free(track);
	active = false;
	playing = false;
	pos = 0;
	trackLength = 0;
}

int64_t Player::getLength() {
	return trackLength;
}

bool Player::isLooping() {
	return looping;
}

void Player::setLooping(bool value) {
	looping = value;
}

void Player::setPlaying(bool value) {
	playing = value;
}