#include "GUI.h"

static bool is_init{};
static bool do_draw{true};

//
// Management functions
//

bool GUI::init(HWND wnd_handle)
{
	if (is_init)
		return false;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(wnd_handle);
	ImGui_ImplOpenGL3_Init();

	is_init = true;

	return false;
}

void GUI::shutdown()
{
	if (!is_init)
		return;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	is_init = false;
}

//
// Global functions
//

void GUI::draw()
{
	if (!do_draw)
		return;

	printf("TEST\n");

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("OpenGL-Hk");
	{
		ImGui::Text("Hello, World!");
	}
	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//
// Get & Set functions
//

bool GUI::getIsInit()
{
	return is_init;
}

bool GUI::getDoDraw()
{
	return do_draw;
}

void GUI::setDoDraw(bool new_value)
{
	do_draw = new_value;
}
