#include <iostream>
#include <minhook/MinHook.h>

typedef BOOL (__stdcall*twglSwapBuffers)(HDC hDc);

twglSwapBuffers originalSwapBuffers = nullptr;

int __stdcall hkSwapBuffers(HDC hDc) {
    std::cout << "Hooked gl function" << std::endl;
    return originalSwapBuffers(hDc);
}


bool setup() {
    if (MH_Initialize() != MH_OK)
        return false;

    MH_STATUS openglHook = MH_CreateHookApi(L"opengl32", "wglSwapBuffers", &hkSwapBuffers,
                                            reinterpret_cast<void **>(&originalSwapBuffers));

    if (openglHook != MH_OK)
        return false;

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
        return false;

    return true;
}

void shutdown() {
    MH_DisableHook(MH_ALL_HOOKS);

    MH_Uninitialize();
}


DWORD __stdcall thread(HMODULE hModule) {
    FILE* file;
    AllocConsole();
    freopen_s(&file, "CONOUT$", "w", stdout);

    std::cout << "Hello World!" << std::endl;

    setup();


    while (true) {
        if (GetAsyncKeyState(VK_END))
            break;

        Sleep(1000);
    }

    shutdown();
    fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) thread, hModule, 0, nullptr);
    }

    if (dwReason == DLL_PROCESS_DETACH) {
        //shutdown();
    }

    return TRUE;
}
