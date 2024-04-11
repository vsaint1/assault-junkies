#include "hook/hook.h"

static unsigned long __stdcall _thread(HMODULE hModule) {

    FILE *file{0};
    AllocConsole();
    freopen_s(&file, "CONOUT$", "w", __acrt_iob_func(1));

    if (!hook::setup())
        spdlog::warn("Failed while initializing hooks");

    spdlog::info("Initialized");

    while (!GetAsyncKeyState(VK_END)) {
        Sleep(1000);
    }

    hook::shutdown();

    fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, NULL);
}

int __stdcall DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    UNREFERENCED_PARAMETER(reserved);

    if (reason == DLL_PROCESS_ATTACH) {

        DisableThreadLibraryCalls(module);

        auto hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)_thread, module, 0, nullptr);

        if (hThread)
            CloseHandle(hThread);
    }

    if (reason == DLL_PROCESS_DETACH)
        return TRUE;

    return TRUE;
}
