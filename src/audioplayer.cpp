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

bool Player::process() {
    memset(buffer, 0, samplesPerTick * sizeof(int16_t) * 2); // Clear the buffer for new audio data
    if (!playing || !frequency) {
        return false; // If not playing, simply return
    } else {
        for (size_t i = 0; i < samplesPerTick; ++i) {
			int16_t sample = playSine();
			
			// Ensure the sample is within the range of int16_t
			if (sample > INT16_MAX) {
				sample = INT16_MAX; // Clamp to maximum
			} else if (sample < INT16_MIN) {
				sample = INT16_MIN; // Clamp to minimum
			}
			
            // Calculate the sample value
            buffer[i*2] = sample;
			buffer[i*2+1] = sample;
        }
    }
    return true; // Indicate that processing was successful
}

int16_t Player::playSine() {
	// Calculate the sample value
	int16_t sample = static_cast<int16_t>(std::sin(phase) * (INT16_MAX/10*volume));
	// Increment phase for the next sample
	phase += (2.0 * M_PI * frequency) / audioSampleRate;

	// Wrap phase if it goes beyond 2 * PI
	if (phase >= 2.0 * M_PI) {
		phase -= 2.0 * M_PI;
	}
	
	return sample;
}

int16_t Player::playSquare() {
    int16_t sample = (std::sin(phase) >= 0) ? (INT16_MAX /10 * volume) : (-INT16_MAX /10* volume);
    phase += (2.0 * M_PI * frequency) / audioSampleRate;

    if (phase >= 2.0 * M_PI) {
        phase -= 2.0 * M_PI;
    }

    return sample;
}

int16_t Player::playSawtooth() {
    double normalizedSample = (phase / (2.0 * M_PI)) * 2.0 - 1.0; // Normalized range from -1 to 1
    int16_t sample = static_cast<int16_t>(normalizedSample * (INT16_MAX/10 * volume));
    
    phase += (2.0 * M_PI * frequency) / audioSampleRate;

    if (phase >= 2.0 * M_PI) {
        phase -= 2.0 * M_PI;
    }

    return sample;
}

int16_t Player::playTriangle() {
    double normalizedSample = (1.0 - std::abs(2.0 * (phase / (2.0 * M_PI) - 0.5))); // Normalized triangle wave
    int16_t sample = static_cast<int16_t>(normalizedSample * (INT16_MAX/10 * volume));

    phase += (2.0 * M_PI * frequency) / audioSampleRate;

    if (phase >= 2.0 * M_PI) {
        phase -= 2.0 * M_PI;
    }

    return sample;
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