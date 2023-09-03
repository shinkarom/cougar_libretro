#include "audioplayer.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include "stb_vorbis.h"

Player::Player() {
	active = false;
	playing = false;
	volume = 100;
}

Player::~Player() {
	
}
void Player::loadTrack(int16_t* tr, int length) {
	track = tr;
	trackLength = length;
	loopStart = 0;
	loopEnd = trackLength;
	active = true;
	playing = true;
	looping = false;
	pos = 0;
}

bool Player::process() {
	memset(buffer, 0, samplesPerTick * 2);
	if(!playing) {
		return false;
	} else {
		auto finalPos = looping ? loopEnd : trackLength;
		auto remaining = finalPos - pos;
		auto samplesNow = remaining < samplesPerTick ? remaining : samplesPerTick;
		for(int i = 0; i<samplesNow; i++) {
			buffer[i*2] = track[pos] / 100*volume;
			buffer[i*2+1] = track[pos]/ 100*volume;
			pos++;
		}
		if(samplesNow < samplesPerTick) {
			if(looping) {
				pos = loopStart;
			} else {
				playing = false;
				pos = 0;
			}
		}
	}
	return true;
}

void Player::seek(int position) {
	pos = position;
}

int Player::tell() {
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

void Player::setVolume(int value) {
	if(value <0 || value > 100) {
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

int64_t Player::getLoopStart() {
	return loopStart;
}

int64_t Player::getLoopEnd() {
	return loopEnd;
}

void Player::setLoopStart(int64_t value) {
	if(value >= 0 && value < loopEnd) {
		loopStart = value;
	}
}

void Player::setLoopEnd(int64_t value) {
	if(value <= trackLength && value > loopStart) {
		loopEnd = value;
	}
}

bool Player::getLooping() {
	return looping;
}

void Player::setLooping(bool value) {
	looping = true;
}

void Player::start() {
	playing = true;
}

void Player::stop() {
	playing = false;
}