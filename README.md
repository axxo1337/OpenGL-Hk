# OpenGL-Hk
This is a POC of how to hook OpenGL I made because of how many people came to me after watching one of my videos about it. <br />
TO NOTE : Make sure to test on x64 games only <br /> <br />
![Screenshot](/repo/screen.png "Screenshot")

## How it works?
All it does is get the addr of the function SwapBuffers using the export symbol from `opengl32.dll`.

You can use the WinAPI function <a href="https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress">GetProcAddress</a> to do so.

## How can I test it?
Well simply build using vs compiler and inject the dll in an OpenGL game.
<br />
Added Linux support ;)
