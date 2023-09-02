#include "audioplayer.h"

#include <cstring>
#include "stb_vorbis.h"

Player::Player() {
	active = false;
	playing = false;
	volume = 100;
}

Player::~Player() {
	
}
void Player::loadTrack(int16_t* tr) {
	track = tr;
	pos = 0;
	active = true;
	playing = true;
}

int16_t* Player::process() {
	memset(buffer, 0, samplesPerTick * 2);
	if(!playing) {
		
	} else {
		for(int i = 0; i<samplesPerTick; i++) {
			buffer[i*2] = track[pos] / 100*volume;
			buffer[i*2+1] = track[pos]/ 100*volume;
			pos++;
		}
	}
			
	return buffer;
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