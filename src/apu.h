#pragma once

#include <cstdint>

namespace apu {
	
	constexpr auto audioSampleRate = 11025;
	constexpr auto samplesPerTick = audioSampleRate / 60;
	constexpr auto maxPlayers = 4;
	
	void init();
	void deinit();
	
	uint16_t* process();
	
	void playFile(int playerNum, const char* fileName);
}