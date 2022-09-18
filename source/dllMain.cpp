#include <thread>
#include <chrono>

#include <Windows.h>

#include "Hooks.h"

void __stdcall MainThread(HMODULE hMod) 
{
	hooks = std::make_unique<Hooks>("Minecraft 1.19.2");

	while (!GetAsyncKeyState(VK_END)) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

	hooks->Remove();
	FreeLibraryAndExitThread(hMod, 0);
}

bool __stdcall DllMain(HINSTANCE hinstDLL, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hinstDLL, 0, 0));
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		FreeConsole();
		fclose(stdout);
	}
	return true;
}