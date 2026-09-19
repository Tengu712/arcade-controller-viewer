#include "bridge.hpp"

#include <algorithm>

RenderingInfo Bridge::bridge(const InputState &input) {
	// register buttons mapping
	for (size_t i = 0; i < input.buttons.max_size() && _map.size() < 8; ++i) {
		if (!input.buttons[i] || std::find(_map.cbegin(), _map.cend(), i) != _map.cend()) {
			continue;
		}
		_map.emplace_back(i);
	}

	for (size_t i = 0; i < _map.size(); ++i) {
		_buttons[i] = input.buttons[_map[i]];
	}

	return {
		input.axisX == AxisXInput::Left
			? -1
			: input.axisX == AxisXInput::Right
			? 1
			: 0,
		input.axisY == AxisYInput::Up
			? -1
			: input.axisY == AxisYInput::Down
			? 1
			: 0,
		_buttons,
	};
}
