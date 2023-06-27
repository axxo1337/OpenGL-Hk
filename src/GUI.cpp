#include "GUI.h"

void GUI::PrivateInit(HWND& hWnd)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplOpenGL3_Init();
}

void GUI::PrivateDestroy()
{
	draw = false;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GUI::Draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (draw)
	{
		ImGui::Begin("OpenGL-Hk");
		{
			ImGui::Text("Hello, World!");
		}
		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//
// Singleton stuff (ignore)
//

GUI* GUI::p_instance;
std::mutex GUI::mutex;

void GUI::Init(HWND& hWnd)
{
	if (p_instance != nullptr)
		return;

	p_instance = new GUI(hWnd);
}

void GUI::Destroy()
{
	if (p_instance == nullptr)
		return;

	p_instance->PrivateDestroy();

	delete p_instance;
	p_instance = nullptr;
}

GUI* GUI::Get()
{
	std::lock_guard<std::mutex> lock(mutex);

	if (p_instance == nullptr)
		return nullptr; // should init but for practical reasons does not

	return p_instance;
}

bool GUI::IsInit()
{
	return p_instance != nullptr;
}