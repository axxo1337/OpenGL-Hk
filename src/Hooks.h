#ifndef HOOKS_H_
#define HOOKS_H_

#include <iostream>
#include <memory>
#include <mutex>

#include <Windows.h>
#include <gl/GL.h>
#include <MinHook.h>

#include "GUI.h"

//#pragma comment(lib, "libMinHook-x64.lib")
//#pragma comment(lib, "opengl32.lib")

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

class Hooks
{
public:
	Hooks(const Hooks&) = delete;
	void operator=(const Hooks&) = delete;

	static void Init();
	static void Destroy();
	static Hooks* Get();
	static bool IsInit();

private:
	void PrivateInit();
	void PrivateDestroy();

	Hooks()
	{
		PrivateInit();
	}

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
	static bool    __stdcall wglSwapBuffers(HDC);

public:
	HWND hWnd                       = nullptr;
	void* swapBuffers               = nullptr;
	WNDPROC oWndProc                = nullptr;
	twglSwapBuffers oWglSwapBuffers = nullptr;

private:
	static Hooks* p_instance;
	static std::mutex mutex;
};

#endif