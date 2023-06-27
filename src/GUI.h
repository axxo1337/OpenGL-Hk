#ifndef GUI_H_
#define GUI_H_

#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

#include <Windows.h>
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_win32.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

class GUI final
{
public:
	GUI(const GUI&) = delete;
	void operator=(const GUI&) = delete;

	static void Init(HWND& hWnd);
	static void Destroy();
	static GUI* Get();
	static bool IsInit();

	void Draw();

private:
	void PrivateInit(HWND& hWnd);
	void PrivateDestroy();

	GUI(HWND& hWnd)
	{
		PrivateInit(hWnd);
	}

public:
	bool draw = true;

private:
	static GUI* p_instance;
	static std::mutex mutex;
};

#endif