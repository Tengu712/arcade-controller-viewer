#pragma once

#include <unordered_set>
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
	AxisXInput               axisX;
	AxisYInput               axisY;
	std::unordered_set<WORD> buttons;

	InputState():
		axisX(AxisXInput::Neutral),
		axisY(AxisYInput::Neutral),
		buttons{}
	{}

	void sync();
};
