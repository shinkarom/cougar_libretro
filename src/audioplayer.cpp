#include "audioplayer.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cmath>

Player::Player() {
	playing = false;
	volume = 10;
	frequency = 0;
	phase = 0;
}

Player::~Player() {
	
}

/*
bool Player::process() {
	memset(buffer, 0, samplesPerTick * 2);
	if(!playing) {
		return false;
	} else {
		auto remaining = trackLength - pos;
		auto samplesNow = remaining < samplesPerTick ? remaining : samplesPerTick;
		for(int i = 0; i<samplesNow; i++) {		
			auto sample = track[pos];
			buffer[i * 2] = sample;
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
*/

bool Player::process() {
    memset(buffer, 0, samplesPerTick * sizeof(int16_t) * 2); // Clear the buffer for new audio data
    if (!playing || !frequency) {
        return false; // If not playing, simply return
    } else {
        for (size_t i = 0; i < samplesPerTick; ++i) {
            // Calculate the sample value
            buffer[i*2] = static_cast<int16_t>(std::sin(phase) * (INT16_MAX/10*volume));
			buffer[i*2+1] = buffer[i*2];
            // Increment phase for the next sample
            phase += (2.0 * M_PI * frequency) / audioSampleRate;

            // Wrap phase if it goes beyond 2 * PI
            if (phase >= 2.0 * M_PI) {
                phase -= 2.0 * M_PI;
            }
        }
    }
    return true; // Indicate that processing was successful
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

void Player::setFrequency(float value) {
	frequency = value;
	playing = (frequency!=0);
}

float Player::getFrequency() {
	return frequency;
}