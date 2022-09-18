#pragma once
#include <iostream>
#include <memory>
#include <mutex>

#include <Windows.h>
#include <gl/GL.h>
#include <MinHook.h>

#include "GUI.h"

#pragma comment(lib, "libMinHook-x64.lib")
#pragma comment(lib, "opengl32.lib")

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

class Hooks
{
public:
	Hooks(const char* windowName);
	void Remove();
private:
	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
	static bool    __stdcall wglSwapBuffers(HDC);
public:
	HWND hWnd                       = nullptr;
	void* swapBuffers               = nullptr;
	WNDPROC oWndProc                = nullptr;
	twglSwapBuffers oWglSwapBuffers = nullptr;
};

inline std::unique_ptr<Hooks> hooks;