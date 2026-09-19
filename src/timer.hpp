#pragma once

#include <Windows.h>

class Timer {
private:
	const HANDLE _handle;

public:
	Timer(const Timer &) = delete;
	Timer &operator =(const Timer &) = delete;

	Timer();
	~Timer();

	void set() const;
	void wait() const;
};
