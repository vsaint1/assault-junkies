#include "sdk/player.h"
#include "sdk/world.h"
#include "utils.h"

static DWORD __stdcall FakeEntry(HMODULE _hModule) {

    FILE *file{0};
    AllocConsole();
    freopen_s(&file, "CONOUT$", "w", stdout);

    auto moduleBase = (uintptr_t)GetModuleHandle("ac_client.exe");
    spdlog::info("Test ac_base {:#0x}", moduleBase);

    int playerCount = *(int *)(moduleBase + offsets::PLAYER_COUNT);

    CPlayer *localPlayer = *(CPlayer **)(moduleBase + offsets::LOCAL_PLAYER);
    int world = *(int *)(moduleBase + offsets::WORLD);
    while (!GetAsyncKeyState(VK_END)) {

        if (localPlayer == nullptr)
            continue;

        spdlog::info("Health {}, LocalPlayer name {}, weapon name {}, type {}, TeamAreEnemies {}, GameMode {}", localPlayer->GetHealth(), localPlayer->GetPlayerName(), localPlayer->GetWeaponName(), localPlayer->GetWeaponType(), CWorld::TeamAreEnemies(world), CWorld::GetGameModeName(world));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

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