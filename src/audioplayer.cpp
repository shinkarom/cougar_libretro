#include "audioplayer.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include "stb_vorbis.h"

Player::Player() {
	active = false;
	playing = false;
	volume = 10;
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
		auto remaining = trackLength - pos;
		auto samplesNow = remaining < samplesPerTick ? remaining : samplesPerTick;
		for(int i = 0; i<samplesNow; i++) {
			buffer[i*2] = track[pos] / 10*volume;
			buffer[i*2+1] = track[pos]/ 10*volume;
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