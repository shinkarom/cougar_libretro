#include <cstdint>

namespace apu {
	
	constexpr auto audioSampleRate = 44100;
	constexpr auto samplesPerTick = audioSampleRate / 60;
	
	void init();
	void deinit();
	
	uint16_t* process();
}