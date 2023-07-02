#include "GUI.h"

void GUI::ReInit(HWND& hWnd)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplOpenGL3_Init();
}

void GUI::Shutdown()
{
	draw = false;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GUI::Draw()
{
	static RECT originalClip;
    	static bool clipped = false;
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (draw)
	{
		if (clipped)
		{
                	GetClipCursor(&originalClip);
                	ClipCursor(nullptr);
                	clipped = false;
            	}
		ImGui::Begin("OpenGL-Hk");
		{
			ImGui::Text("Hello, World!");
		}
		ImGui::End();
	}
	else if (!clipped)
	{
		ClipCursor(&originalClip);
                clipped = true;
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

	p_instance->Shutdown();

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
