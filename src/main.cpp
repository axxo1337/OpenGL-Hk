#include <thread>
#include <chrono>

#include <Windows.h>

#include "Hook.h"

void __stdcall MainThread(HINSTANCE instance) 
{
	if (Hook::init())
		goto _shutdown;

	while (!GetAsyncKeyState(VK_END)) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

_shutdown:
	Hook::shutdown();
	FreeLibrary(instance);
}

bool __stdcall DllMain(HINSTANCE instance, DWORD reason, LPVOID p_reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

		/* Create, join and detach thread */
		if (static std::thread main_thread([instance] { MainThread(instance); }); main_thread.joinable())
			main_thread.detach();
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		FreeConsole();
		fclose(stdout);
	}

	return true;
}
