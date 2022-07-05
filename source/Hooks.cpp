#include "Hooks.h"

#include <iostream>

#include "GUI.h"

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);
twglSwapBuffers owglSwapBuffers;
WNDPROC oWndProc;
HWND hWnd;

Hooks::Hooks(const char* windowName)
{
    { // Hook WndProc
        while (!hWnd)
            hWnd = FindWindowA(0, windowName);
        oWndProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
    }

    { // Hook OpenGL
        swapBuffers = (void*)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
        std::cout << "[+] SwapBuffers addr : " << (uintptr_t)swapBuffers << std::endl;

        MH_Initialize();
        MH_CreateHook(swapBuffers, &wglSwapBuffers, (LPVOID*)&owglSwapBuffers);
        MH_EnableHook(swapBuffers);
        std::cout << "[+] Hooked OpengGL! Origin : " << (uintptr_t)owglSwapBuffers << std::endl;
    }
}

void Hooks::Remove()
{
    SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
    MH_RemoveHook(swapBuffers);
}

//=================================================
// Begin: Hooked functions
//=================================================

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Change menu state
    if (msg == WM_KEYDOWN && wParam == VK_INSERT)
        gui->draw = !gui->draw;

    if (gui && gui->draw && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return CallWindowProcA(oWndProc, hWnd, msg, wParam, lParam);
}

bool __stdcall Hooks::wglSwapBuffers(HDC hDc)
{
    static HGLRC oContext = wglGetCurrentContext();
    static HGLRC newContext = wglCreateContext(hDc);

    static bool init = false;
    if (!init)
    {
        wglMakeCurrent(hDc, newContext);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0.0, 640, 480, 0.0, 1.0, -1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClearColor(0, 0, 0, 1.0);

        gui = std::make_unique<GUI>(hWnd);

        init = true;
    }
    else
    {
        wglMakeCurrent(hDc, newContext);
        gui->Draw();
    }

    wglMakeCurrent(hDc, oContext);

    return owglSwapBuffers(hDc);
}

//=================================================
// End: Hooked functions
//=================================================