#include "audioplayer.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include "stb_vorbis.h"

Player::Player() {
	active = false;
	playing = false;
	volume = 10;
	pan = 0;
}

Player::~Player() {
	
}

void Player::loadTrack(int16_t* tr, int length) {
	track = tr;
	trackLength = length;
	loopStart = 0;
	loopEnd = trackLength;
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
		if(looping && (pos < loopStart || pos >= loopEnd)) {
			pos = loopStart;
		}
		auto finalPos = looping ? loopEnd : trackLength;
		auto remaining = finalPos - pos;
		auto samplesNow = remaining < samplesPerTick ? remaining : samplesPerTick;
		for(int i = 0; i<samplesNow; i++) {
			buffer[i*2] = track[pos] / 10*volume*(10-pan)/20;
			buffer[i*2+1] = track[pos]/ 10*volume*(pan+10)/20;
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

int64_t Player::getLoopStart() {
	return loopStart;
}

int64_t Player::getLoopEnd() {
	return loopEnd;
}

void Player::setLoopStart(int64_t value) {
	if(value <= loopEnd) {
		loopStart = value;
		if(loopStart < 0) {
			loopStart = 0;
		}
		if(playing && looping && (pos < value)) {
			pos = value;
		}
	}
}

void Player::setLoopEnd(int64_t value) {
	if(value > loopStart) {
		loopEnd = value;
		if(loopEnd > trackLength) {
			loopEnd = trackLength;
		}
		if(playing && looping && (pos > value)) {
			pos = loopStart;
		}
	}
}

bool Player::isLooping() {
	return looping;
}

void Player::setLooping(bool value) {
	looping = value;
	if(playing && ((pos < loopStart) || (pos > loopEnd))) {
		pos = loopStart;
	}
}

void Player::setPlaying(bool value) {
	playing = value;
	if(playing && looping && ((pos < loopStart) || (pos > loopEnd))) {
		pos = loopStart;
	}
}

int Player::getPan() {
	return pan;
}

void Player::setPan(int value) {
if(value >= -10 && value <= 10) {
	pan = value;
}
}