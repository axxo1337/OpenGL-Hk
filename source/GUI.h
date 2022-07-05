#pragma once
#include <memory>

#include <Windows.h>
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_win32.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

class GUI final
{
public:
	GUI(HWND& hWnd);
	void Draw();
public:
	bool draw = true;
};

inline std::unique_ptr<GUI> gui;