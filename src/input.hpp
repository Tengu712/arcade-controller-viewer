#pragma once

#include <unordered_map>
#include <Windows.h>

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
	std::unordered_map<WORD, bool> buttons;
	AxisXInput                     axisX;
	AxisYInput                     axisY;

	InputState():
		buttons{},
		axisX(AxisXInput::Neutral),
		axisY(AxisYInput::Neutral)
	{}
};

InputState getControllerInputState();
