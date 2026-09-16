@echo off

cl /EHsc /Fe:"acv.exe" /std:c++20 /utf-8 /W4 /WX src\main.cpp src\input.cpp user32.lib xinput.lib /link /SUBSYSTEM:WINDOWS
del *.obj
