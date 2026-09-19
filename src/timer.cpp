#include "timer.hpp"

Timer::Timer():
	_handle(CreateWaitableTimerExW(
		NULL,
		NULL,
		CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
		TIMER_ALL_ACCESS
	))
{
	if (!_handle) {
		throw L"failed to create timer";
	}
}

Timer::~Timer() {
	CloseHandle(_handle);
}

void Timer::set() const {
	LARGE_INTEGER time;
	time.QuadPart = -166666;
	SetWaitableTimer(_handle, &time, 0, NULL, NULL, false);
}

void Timer::wait() const {
	WaitForSingleObject(_handle, INFINITE);
}
