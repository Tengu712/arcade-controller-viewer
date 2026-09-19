#pragma once

#include "graphics.hpp"
#include "input.hpp"

#include <array>
#include <vector>

class Bridge {
private:
	std::vector<WORD> _map;
	std::array<bool, 8> _buttons;

public:
	Bridge();

	RenderingInfo bridge(const InputState &input);
};
