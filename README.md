# OpenGL-Hk
This is a POC of how to hook OpenGL I made because of how many people came to me after watching one of my videos about it.

## How it works?
All it does is get the addr of the function SwapBuffers using the export symbol from `opengl32.dll`.

You can use the WinAPI function GetProcAddress to do so.

## How can I test it?
Well simply build using vs compiler and inject the dll in an OpenGL game.
