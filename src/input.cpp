#include "input.hpp"

#include <Windows.h>

#include <xinput.h>

bool InputState::sync() {
	auto changed = false;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	if (XInputGetState(0, &state) != ERROR_SUCCESS) {
		return changed;
	}

	const auto x =
		state.Gamepad.sThumbLX < -30000
			? AxisXInput::Left
			: state.Gamepad.sThumbLX > 30000
			? AxisXInput::Right
			: AxisXInput::Neutral;
	const auto y =
		state.Gamepad.sThumbLY < -30000
			? AxisYInput::Down
			: state.Gamepad.sThumbLY > 30000
			? AxisYInput::Up
			: AxisYInput::Neutral;

	if (x != axisX || y != axisY) {
		changed = true;
		axisX = x;
		axisY = y;
	}

	for (size_t i = 0; i < buttons.max_size(); ++i) {
		const auto k = static_cast<WORD>(1 << i);
		const auto v = static_cast<bool>(state.Gamepad.wButtons & k);

		if (v != buttons[i]) {
			changed = true;
			buttons[i] = v;
		}
	}

	return changed;
}
