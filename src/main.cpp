#include <thread>
#include <chrono>

#include <Windows.h>

#include "Hooks.h"

void __stdcall MainThread(HINSTANCE instance) 
{
	Hooks::Init();

	while (!GetAsyncKeyState(VK_END)) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

	Hooks::Destroy();
	FreeLibrary(instance);
}

bool __stdcall DllMain(HINSTANCE instance, DWORD reason, LPVOID p_reserved)
{
	static std::thread main_thread;

	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

		main_thread = std::thread([instance] { MainThread(instance); });
		if (main_thread.joinable())
			main_thread.detach();
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		FreeConsole();
		fclose(stdout);
	}
	return true;
}
