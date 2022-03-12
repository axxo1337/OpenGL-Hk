#pragma once
#include <MinHook.h>
#include <gl/GL.h>

#pragma comment(lib, "libMinHook-x64.lib")
#pragma comment(lib, "opengl32.lib")

class Hook
{
	Hook() {}
	static Hook hooks;
public:
	Hook(const Hook&) = delete;
	static Hook& Get() { return hooks; }

	void Install();
	void Remove();
private:
	static bool __stdcall wglSwapBuffers(HDC hDc);
private: 
	void* swapBuffers;
};

