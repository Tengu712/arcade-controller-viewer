#pragma once

#include <array>

enum class AxisXInput {
	Neutral,
	Left,
	Right,
};

enum class AxisYInput {
	Neutral,
	Up,
	Down,
};

struct InputState {
	AxisXInput           axisX;
	AxisYInput           axisY;
	std::array<bool, 16> buttons;

	InputState():
		axisX(AxisXInput::Neutral),
		axisY(AxisYInput::Neutral),
		buttons{}
	{}

	bool sync();
};
