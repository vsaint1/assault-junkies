#include "mem/memory.h"
#include "sdk/player.h"
#include "sdk/world.h"
#include "utils.h"

typedef int(__stdcall *wglSwapBuffers)(HDC);

wglSwapBuffers swapBuffers_Original = nullptr;
int __stdcall wglSwapBuffers_Hook(HDC _hdc) {

    // spdlog::info("Test hook");
    return swapBuffers_Original(_hdc);
}

static DWORD __stdcall FakeEntry(HMODULE _hModule) {

    FILE *file{0};
    AllocConsole();
    freopen_s(&file, "CONOUT$", "w", stdout);

    auto moduleBase = (uintptr_t)GetModuleHandle("ac_client.exe");
    spdlog::info("Test ac_base {:#0x}", moduleBase);

    int playerCount = *(int *)(moduleBase + offsets::PLAYER_COUNT);

    CPlayer *localPlayer = *(CPlayer **)(moduleBase + offsets::LOCAL_PLAYER);

    MH_Initialize();


    if (MH_CreateHookApi(L"opengl32.dll", "wglSwapBuffers", (void **)&wglSwapBuffers_Hook, (void **)&swapBuffers_Original) != MH_OK)
        spdlog::error("Failed to hook wglSwapBuffers");

    MH_EnableHook(MH_ALL_HOOKS);

    // swapBuffers_Original = (wglSwapBuffers)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    // swapBuffers_Original = (wglSwapBuffers)memory::Trampoline((char *)swapBuffers_Original, (char *)wglSwapBuffers_Hook, 5);

    // memory::Write<int>(moduleBase + offsets::WORLD,20);

    while (!GetAsyncKeyState(VK_END)) {

        if (localPlayer == nullptr)
            continue;

        int world = *(int *)(moduleBase + offsets::WORLD);

        spdlog::info("Health {}, LocalPlayer name {}, weapon name {}, type {}, TeamAreEnemies {}, \n GameMode {}", localPlayer->GetHealth(), localPlayer->GetPlayerName(), localPlayer->GetWeaponName(), localPlayer->GetWeaponType(), CWorld::TeamAreEnemies(world), CWorld::GetGameModeName(world));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread(_hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE _hModule, DWORD ul_reason_for_call, LPVOID _lpReserved) {
    UNREFERENCED_PARAMETER(_lpReserved);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

        DisableThreadLibraryCalls(_hModule);
        auto hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FakeEntry, _hModule, 0, nullptr);

        if (hThread)
            CloseHandle(hThread);
    }

    return TRUE;
}