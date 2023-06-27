#include "Hooks.h"

void Hooks::PrivateInit()
{
    // To get window handle
    hWnd = FindWindowA("LWJGL", nullptr); // older versions
    if (!hWnd)
        hWnd = FindWindowA("GLFW30", nullptr); // more recent versions of mc

    if (!hWnd)
    {
        printf("[-] Failed to get window handle\n");
        return;
    }

    oWndProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

    MH_Initialize();

    swapBuffers = (void*)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");

    if (!swapBuffers)
    {
        printf("[-] Failed to get swapbuffers\n");
        return;
    }

    MH_CreateHook(swapBuffers, &wglSwapBuffers, (LPVOID*)&oWglSwapBuffers);

    MH_EnableHook(MH_ALL_HOOKS);
}

void Hooks::PrivateDestroy()
{
    SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);

    MH_DisableHook(MH_ALL_HOOKS);
    GUI::Get()->Destroy();
    MH_RemoveHook(MH_ALL_HOOKS);
}

//
// Hooked functions
//

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (GUI::Get())
    {
        // Change menu state
        if (msg == WM_KEYDOWN && wParam == VK_INSERT)
            GUI::Get()->draw = !GUI::Get()->draw;

        if (GUI::Get()->draw && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;
    }

    return CallWindowProcA(Hooks::Get()->oWndProc, hWnd, msg, wParam, lParam);
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

        GUI::Init(Hooks::Get()->hWnd);

        init = true;
    }
    else
    {
        wglMakeCurrent(hDc, newContext);
        GUI::Get()->Draw();
    }

    wglMakeCurrent(hDc, oContext);

    return Hooks::Get()->oWglSwapBuffers(hDc);
}

//
// Singleton stuff (ignore)
//

Hooks* Hooks::p_instance;
std::mutex Hooks::mutex;

void Hooks::Init()
{
    if (p_instance != nullptr)
        return;

    p_instance = new Hooks;
}

void Hooks::Destroy()
{
    if (p_instance == nullptr)
        return;

    p_instance->PrivateDestroy();

    delete p_instance;
    p_instance = nullptr;
}

Hooks* Hooks::Get()
{
    std::lock_guard<std::mutex> lock(mutex);

    if (p_instance == nullptr)
        return nullptr; // should init but for practical reasons does not

    return p_instance;
}

bool Hooks::IsInit()
{
    return p_instance != nullptr;
}