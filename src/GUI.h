#ifndef GUI_H_
#define GUI_H_

#include <cstdio>

#include <Windows.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace GUI
{
	//
	// Management functions
	//

	bool init(HWND wnd_handle);
	void shutdown();

	//
	// Global functions
	//

	void draw();

	//
	// Get & Set functions
	//

	bool getIsInit();
	bool getDoDraw();

	void setDoDraw(bool new_value);
}

#endif