#include "Hook.h"

#include <iostream>

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);
twglSwapBuffers owglSwapBuffers;

void Hook::Install() {
	swapBuffers = (void*)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
	std::cout << "[+] SwapBuffers addr : " << (uintptr_t)swapBuffers << std::endl;

	MH_Initialize();
	MH_CreateHook(swapBuffers, &wglSwapBuffers, (LPVOID*)&owglSwapBuffers);
	MH_EnableHook(swapBuffers);
	std::cout << "[+] Hooked! Origin : " << (uintptr_t)owglSwapBuffers << std::endl;
}

void Hook::Remove() {
	MH_RemoveHook(swapBuffers);
}

bool __stdcall Hook::wglSwapBuffers(HDC hDc)
{
    static HGLRC oContext = wglGetCurrentContext();
    static HGLRC newContext = wglCreateContext(hDc);

    static bool init = false;
    if (!init) {
        wglMakeCurrent(hDc, newContext);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0.0, 640, 480, 0.0, 1.0, -1.0);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClearColor(0, 0, 0, 1.0);

        init = true;
    }
    else {
        wglMakeCurrent(hDc, newContext);

        const float x = 100;
        const float y = 100;

        const float w = 90;
        const float h = 90;

        glColor3f(1.0f, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y+h);
        glVertex2f(x, y+h);
        glEnd();
    }

    wglMakeCurrent(hDc, oContext);

	return owglSwapBuffers(hDc);
}

Hook Hook::hooks;