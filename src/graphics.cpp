#include "graphics.hpp"

#include "input.hpp"

#include <cmath>
#include <utility>
#include <uxtheme.h>

namespace {
	const std::array<std::pair<int, int>, 8> BUTTON_POSITIONS{
		std::make_pair(100, 32),
		std::make_pair(140, 18),
		std::make_pair(180, 16),
		std::make_pair(220, 14),
		std::make_pair( 95, 74),
		std::make_pair(135, 60),
		std::make_pair(175, 58),
		std::make_pair(215, 56),
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

void Graphics::_draw(HDC hdc, RenderingInfo &info) const {
	Gdiplus::Graphics g(hdc);
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	// background
	g.FillRectangle(&_brushBG, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);

	// lever
	const auto k =
		info.leverX * info.leverX > 0.0 && info.leverY * info.leverY > 0.0
			? 1.0 / std::sqrt(2.0)
			: 1.0;
	g.FillEllipse(&_brushGray, 30, 53, 32, 32);
	g.FillEllipse(
		&_brushWight,
		static_cast<int>(30.0 + 16.0 * info.leverX * k),
		static_cast<int>(53.0 + 16.0 * info.leverY * k),
		32,
		32
	);

	// buttons
	for (size_t i = 0; i < BUTTON_POSITIONS.size(); ++i) {
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
