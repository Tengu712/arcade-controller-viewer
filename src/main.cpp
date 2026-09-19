#include "bridge.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "timer.hpp"

constexpr LPCWSTR WINDOW_CLASS_NAME = L"arcade-controller-viewer";

struct Context {
	Graphics graphics;
	InputState input;
	Bridge bridge;
};

LRESULT CALLBACK processWindowMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto context = reinterpret_cast<Context *>(GetWindowLongPtrW(window, GWLP_USERDATA));

	switch (msg) {
	case WM_NCCREATE: {
		const auto cs = reinterpret_cast<CREATESTRUCTW *>(lParam);
		const auto lp = reinterpret_cast<LONG_PTR>(cs->lpCreateParams);
		SetWindowLongPtrW(window, GWLP_USERDATA, lp);
		break;
	}
	case WM_CLOSE:
		DestroyWindow(window);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT:
		if (context) {
			context->graphics.draw(window, context->bridge.bridge(context->input));
			return 0;
		}
		break;
	}

	return DefWindowProcW(window, msg, wParam, lParam);
}

void registerWindowClass(HINSTANCE instance) {
	WNDCLASSEXW wndCls;
	if (GetClassInfoExW(instance, WINDOW_CLASS_NAME, &wndCls)) {
		return;
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

	if (!RegisterClassExW(&wndCls)) {
		throw L"failed to register window class";
	}
}

HWND createWindow(HINSTANCE instance, Context *context) {
	const auto window = CreateWindowExW(
		WS_EX_TOOLWINDOW,
		WINDOW_CLASS_NAME,
		L"arcade controller viewer",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		0,
		0,
		CLIENT_WIDTH,
		CLIENT_HEIGHT,
		NULL,
		NULL,
		instance,
		context
	);

	if (!window) {
		throw L"failed to create window";
	}

	RECT rc{0, 0, CLIENT_WIDTH, CLIENT_HEIGHT};
	if (!AdjustWindowRectEx(
		&rc,
		GetWindowLongW(window, GWL_STYLE),
		GetMenu(window) != nullptr,
		GetWindowLongW(window, GWL_EXSTYLE)
	)) {
		throw L"failed to adjust window size";
	}
	if (!SetWindowPos(
		window,
		nullptr,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER
	)) {
		throw L"failed to resize window";
	}

	return window;
}

void run(HINSTANCE instance) {
	Context context;
	const Timer timer;

	registerWindowClass(instance);
	const auto window = createWindow(instance, &context);

	timer.set();

	MSG msg;
	while (true) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			continue;
		}

		timer.wait();

		if (context.input.sync()) {
			RedrawWindow(window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}

		timer.set();
	}

	UnregisterClassW(WINDOW_CLASS_NAME, instance);
}

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) {
	try {
		run(instance);
		return 0;
	} catch (LPCWSTR msg) {
		MessageBoxW(NULL, msg, L"Error", MB_OK);
		return 1;
	}
}
