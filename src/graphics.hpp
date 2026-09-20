#pragma once

#include <array>
#include <Windows.h>

#include <gdiplus.h>

constexpr int CLIENT_WIDTH  = 266;
constexpr int CLIENT_HEIGHT = 120;

struct RenderingInfo {
	const double leverX; // -1.0 or 0.0 or 1.0
	const double leverY; // -1.0 or 0.0 or 1.0
	const std::array<bool, 8> &buttons;

	RenderingInfo(double leverX, double leverY, const std::array<bool, 8> &buttons):
		leverX(leverX),
		leverY(leverY),
		buttons(buttons)
	{}
};

class GdiplusSession {
private:
	const ULONG_PTR _token;

public:
	GdiplusSession(const GdiplusSession &) = delete;
	GdiplusSession &operator =(const GdiplusSession &) = delete;

	GdiplusSession();
	~GdiplusSession();
};

class Graphics {
private:
	const GdiplusSession _session;
	const Gdiplus::SolidBrush _brushBG;
	const Gdiplus::SolidBrush _brushGray;
	const Gdiplus::SolidBrush _brushWight;

	void _draw(HDC hdc, RenderingInfo &info) const;

public:
	Graphics():
		_session(),
		_brushBG   (Gdiplus::Color(255,   0,   0,   0)),
		_brushGray (Gdiplus::Color(255, 128, 128, 128)),
		_brushWight(Gdiplus::Color(255, 255, 255, 255))
	{}

	void draw(HWND window, RenderingInfo info) const;
};
