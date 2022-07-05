#pragma once
#include <memory>

#include <Windows.h>
#include <gl/GL.h>
#include <MinHook.h>
#include <Imgui/imgui.h>
#include <Imgui/backends/imgui_impl_win32.h>
#include <Imgui/backends/imgui_impl_opengl3.h>

#pragma comment(lib, "libMinHook-x64.lib")
#pragma comment(lib, "opengl32.lib")

class Hooks
{
public:
	Hooks(const char* windowName);
	void Remove();
private:
	static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool __stdcall wglSwapBuffers(HDC hDc);
private:
	void* swapBuffers;
};

inline std::unique_ptr<Hooks> hooks;