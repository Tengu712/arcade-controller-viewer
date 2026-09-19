#include "bridge.hpp"

#include <algorithm>

Bridge::Bridge():
	_map{},
	_buttons{false, false, false, false, false, false, false, false}
{
	_map.reserve(8);
}

RenderingInfo Bridge::bridge(const InputState &input) {
	// register buttons mapping
	if (_map.size() < 8) {
		for (const auto k: input.buttons) {
			if (_map.size() >= 8) {
				break;
			}
			if (std::find(_map.cbegin(), _map.cend(), k) != _map.cend()) {
				continue;
			}
			_map.emplace_back(k);
		}
	}

	for (size_t i = 0; i < _map.size(); ++i) {
		_buttons[i] = input.buttons.contains(_map[i]);
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
