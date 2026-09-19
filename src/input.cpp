#include "input.hpp"

#include <xinput.h>

void InputState::sync() {
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	if (XInputGetState(0, &state) != ERROR_SUCCESS) {
		return;
	}

	axisX =
		state.Gamepad.sThumbLX < -30000
			? AxisXInput::Left
			: state.Gamepad.sThumbLX > 30000
			? AxisXInput::Right
			: AxisXInput::Neutral;
	axisY =
		state.Gamepad.sThumbLY < -30000
			? AxisYInput::Down
			: state.Gamepad.sThumbLY > 30000
			? AxisYInput::Up
			: AxisYInput::Neutral;

	buttons.clear();

	for (DWORD b = 1; b <= 0x8000; b *= 2) {
		const auto k = static_cast<WORD>(b);
		if (static_cast<bool>(state.Gamepad.wButtons & k)) {
			buttons.emplace(k);
		}
	}
}
