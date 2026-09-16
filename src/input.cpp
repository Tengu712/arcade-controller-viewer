#include "input.hpp"

#include <xinput.h>

InputState getControllerInputState() {
	InputState result;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	if (XInputGetState(0, &state) != ERROR_SUCCESS) {
		return result;
	}

	for (DWORD b = 1; b <= 0x8000; b *= 2) {
		const auto k = static_cast<WORD>(b);
		result.buttons[k] = static_cast<bool>(state.Gamepad.wButtons & k);
	}

	result.axisX =
		state.Gamepad.sThumbLX < -30000
			? AxisXInput::Left
			: state.Gamepad.sThumbLX > 30000
			? AxisXInput::Right
			: AxisXInput::Neutral;
	result.axisY =
		state.Gamepad.sThumbLY < -30000
			? AxisYInput::Down
			: state.Gamepad.sThumbLY > 30000
			? AxisYInput::Up
			: AxisYInput::Neutral;

	return result;
}
