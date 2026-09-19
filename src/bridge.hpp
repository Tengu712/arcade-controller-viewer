#pragma once

#include "graphics.hpp"
#include "input.hpp"

#include <array>
#include <vector>

class Bridge {
private:
	std::vector<size_t> _map;
	std::array<bool, 8> _buttons;

public:
	Bridge():
		_map{},
		_buttons{}
	{
		_map.reserve(8);
	}

	RenderingInfo bridge(const InputState &input);
};
