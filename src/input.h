#include <cstdint>

namespace input {
	
	constexpr auto numButtons = 12;
	
	void init();
	void deinit();
	uint32_t pressedButtons();
	uint32_t releasedButtons();
	bool isButtonPressed(int num);
	bool isButtonReleased(int num);
	
	void setPressedButtons(uint32_t value);
	
}