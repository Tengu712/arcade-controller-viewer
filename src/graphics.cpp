#include "graphics.hpp"

#include "input.hpp"

#include <utility>
#include <uxtheme.h>

namespace {
	const std::array<std::pair<int, int>, 8> BUTTON_POSITIONS{
		std::make_pair(100, 34),
		std::make_pair(140, 20),
		std::make_pair(180, 18),
		std::make_pair(220, 16),
		std::make_pair( 95, 76),
		std::make_pair(135, 62),
		std::make_pair(175, 60),
		std::make_pair(215, 58),
	};

	ULONG_PTR createGdiPlusToken() {
		Gdiplus::GdiplusStartupInput gsi;
		ULONG_PTR token;

		if (Gdiplus::GdiplusStartup(&token, &gsi, NULL) != Gdiplus::Ok) {
			throw L"failed to start up Gdiplus";
		}
		return token;
	}
}

GdiplusSession::GdiplusSession():
	_token(createGdiPlusToken())
{}

GdiplusSession::~GdiplusSession() {
	Gdiplus::GdiplusShutdown(_token);
}

Graphics::Graphics():
	_brushBG   (Gdiplus::Color(255,   0,   0,   0)),
	_brushGray (Gdiplus::Color(255, 128, 128, 128)),
	_brushWight(Gdiplus::Color(255, 255, 255, 255))
{}

void Graphics::_draw(HDC hdc, RenderingInfo info) const {
	Gdiplus::Graphics g(hdc);
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	// back ground
	g.FillRectangle(&_brushBG, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// lever
	g.FillEllipse(&_brushGray, 30, 55, 32, 32);
	g.FillEllipse(&_brushWight, 30 + 16 * info.leverX, 55 + 16 * info.leverY, 32, 32);

	// buttons
	for (int i = 0; i < BUTTON_POSITIONS.size(); ++i) {
		g.FillEllipse(
			info.buttons[i] ? &_brushWight : &_brushGray,
			BUTTON_POSITIONS[i].first,
			BUTTON_POSITIONS[i].second,
			32,
			32
		);
	}
}

void Graphics::draw(HWND window, RenderingInfo info) const {
	PAINTSTRUCT ps;
	const auto hdc = BeginPaint(window, &ps);
	HDC hdcBuf;
	const auto buffer = BeginBufferedPaint(hdc, &ps.rcPaint, BPBF_COMPATIBLEBITMAP, NULL, &hdcBuf);

	_draw(hdcBuf, info);

	EndBufferedPaint(buffer, TRUE);
	EndPaint(window, &ps);
}
