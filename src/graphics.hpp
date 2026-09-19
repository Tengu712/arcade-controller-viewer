#pragma once

#include <Windows.h>

#include <gdiplus.h>

constexpr int WINDOW_WIDTH  = 320;
constexpr int WINDOW_HEIGHT = 180;

class GdiplusSession {
private:
	const ULONG_PTR _token;

public:
	GdiplusSession();
	~GdiplusSession();
};

class Graphics {
private:
	const GdiplusSession _session;
	const Gdiplus::SolidBrush _brushBG;
	const Gdiplus::SolidBrush _brushGray;
	const Gdiplus::SolidBrush _brushWight;

public:
	Graphics();

	void draw(HWND window) const;
};
