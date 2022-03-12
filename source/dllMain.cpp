#include <thread>
#include <chrono>

#include <Windows.h>

#include "Hook.h"

void __stdcall MainThread(HMODULE hMod) {
	Hook::Get().Install();

	while (!GetAsyncKeyState(VK_END)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

	Hook::Get().Remove();
	FreeLibraryAndExitThread(hMod, 0);
}

bool __stdcall DllMain(HINSTANCE hinstDLL, DWORD reason, LPVOID lpReserved)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hinstDLL, 0, 0));
	} break;

	case DLL_PROCESS_DETACH: {
		FreeConsole();
		fclose(stdout);
	} break;
	}
	return true;
}