#pragma once
#include <memory>
#include <thread>
#include <chrono>

#include <Windows.h>
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_win32.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#include "Hooks.h"

class GUI final
{
public:
	GUI(HWND& hWnd);
	void Draw();
	void Remove();
public:
	bool draw = true;
};

inline std::unique_ptr<GUI> gui;