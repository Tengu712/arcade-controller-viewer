#include "input.hpp"

#include <iostream>

constexpr LPCWSTR WINDOW_CLASS_NAME = L"arcade-controller-viewer";

LRESULT processWindowMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcW(window, msg, wParam, lParam);
	}
}

// TODO: draw

bool registerWindowClass(HINSTANCE instance) {
	WNDCLASSEXW wndCls;
	if (GetClassInfoExW(instance, WINDOW_CLASS_NAME, &wndCls)) {
		return false;
	}

	wndCls.cbSize        = sizeof(WNDCLASSEXW);
	wndCls.style         = CS_BYTEALIGNCLIENT;
	wndCls.lpfnWndProc   = processWindowMessage;
	wndCls.cbClsExtra    = 0;
	wndCls.cbWndExtra    = 0;
	wndCls.hInstance     = instance;
	wndCls.hIcon         = NULL;
	wndCls.hCursor       = NULL;
	wndCls.hbrBackground = NULL;
	wndCls.lpszMenuName  = NULL;
	wndCls.lpszClassName = WINDOW_CLASS_NAME;
	wndCls.hIconSm       = NULL;

	return RegisterClassExW(&wndCls) != 0;
}

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showingStyle) {
	if (!registerWindowClass(instance)) {
		MessageBoxW(NULL, L"failed to register window class", L"Error", MB_OK);
		return 1;
	}

	const auto window = CreateWindowExW(
		WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		WINDOW_CLASS_NAME,
		L"arcade controller viewer",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		0,
		0,
		320,
		180,
		NULL,
		NULL,
		instance,
		NULL
	);
	if (!window) {
		MessageBoxW(NULL, L"failed to create window", L"Error", MB_OK);
		return 1;
	}
	if (!ShowWindow(window, showingStyle)) {
		MessageBoxW(NULL, L"failed to show window", L"Error", MB_OK);
		return 1;
	}

	const auto timer = CreateWaitableTimerExW(
		NULL,
		NULL,
		CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
		TIMER_ALL_ACCESS
	);
	if (!timer) {
		MessageBoxW(NULL, L"failed to create timer", L"Error", MB_OK);
		return 1;
	}

	MSG msg;
	while (true) {
		if (PeekMessageW(&msg, window, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			continue;
		}

		// TODO:
		const auto inputState = getControllerInputState();
		(void)inputState;

		LARGE_INTEGER time;
		time.QuadPart = -166666;
		SetWaitableTimer(timer, &time, 0, NULL, NULL, false);
		WaitForSingleObject(timer, INFINITE);
	}

	CloseHandle(timer);
	UnregisterClassW(WINDOW_CLASS_NAME, instance);
	return 0;
}
