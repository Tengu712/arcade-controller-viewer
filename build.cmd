@echo off

cl ^
	/EHsc ^
	/utf-8 ^
	/std:c++20 ^
	/W4 /WX ^
	/Fe:"acv.exe" ^
	src\graphics.cpp ^
	src\input.cpp ^
	src\main.cpp ^
	gdiplus.lib ^
	user32.lib ^
	xinput.lib ^
	/link ^
	/SUBSYSTEM:WINDOWS
del *.obj
