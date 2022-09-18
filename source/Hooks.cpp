#include "Hooks.h"

Hooks::Hooks(const char* windowName)
{
    // Hook WndProc
    while (!hWnd)
        hWnd = FindWindowA(0, windowName);
    oWndProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
    
    MH_Initialize();

    // Hook OpenGL
    swapBuffers = (void*)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
    std::cout << "[+] SwapBuffers addr : " << (uintptr_t)swapBuffers << std::endl;

    MH_CreateHook(swapBuffers, &wglSwapBuffers, (LPVOID*)&oWglSwapBuffers);
    MH_EnableHook(swapBuffers);
    std::cout << "[+] Hooked OpengGL! Origin : " << (uintptr_t)oWglSwapBuffers << std::endl;
}

void Hooks::Remove()
{
    SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);

    gui->Remove();
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

    return CallWindowProcA(hooks->oWndProc, hWnd, msg, wParam, lParam);
}

bool __stdcall Hooks::wglSwapBuffers(HDC hDc)
{
    static HGLRC oContext = wglGetCurrentContext();
    static HGLRC newContext = wglCreateContext(hDc);

    static bool init = false;
    if (!init)
    {
        newContext = wglCreateContext(hDc);
        wglMakeCurrent(hDc, newContext);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(0, 0, viewport[2], viewport[3]);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);

        gui = std::make_unique<GUI>(hooks->hWnd);

        init = true;
    }
    else
    {
        wglMakeCurrent(hDc, newContext);
        gui->Draw();
    }

    wglMakeCurrent(hDc, oContext);

    return hooks->oWglSwapBuffers(hDc);
}

//=================================================
// End: Hooked functions
//=================================================