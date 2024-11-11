#include "mem/memory.h"
#include "render/opengl_hook.h"
#include "sdk/player.h"
#include "sdk/world.h"

typedef int(__stdcall *wglSwapBuffers)(HDC);
wglSwapBuffers swapBuffers_Original = nullptr;

auto moduleBase = (uintptr_t)GetModuleHandle("ac_client.exe");

int __stdcall wglSwapBuffers_Hook(HDC _hdc) {

    if (render->GetHdc() != _hdc)
        render->BuildFont();

    render->SetupOrtho();

    ViewMatrix vm = *(ViewMatrix *)(moduleBase + offsets::VIEWMATRIX);

    uintptr_t *entityList = (uintptr_t *)(moduleBase + offsets::ENTITY_LIST);

    int *playerCount = (int *)(moduleBase + offsets::PLAYER_COUNT);

    CPlayer *localPlayer = (CPlayer *)(moduleBase + offsets::LOCAL_PLAYER);

    // spdlog::info(vm.Debug().str());

    for (size_t i = 0; i < (*playerCount - 1); i++) {

        if (localPlayer == nullptr)
            continue;

        if (entityList == nullptr)
            continue;

        uintptr_t *listEntry = (uintptr_t *)(*entityList + (i * 0x4));

        if (listEntry == nullptr)
            continue;

        CPlayer *entity = (CPlayer *)(*listEntry);

        if (!entity->IsValid())
            continue;

        auto w2s = entity->GetHeadPos().WorldToScreen(vm, render->GetWidth(), render->GetHeight());

        render->Text({w2s.x - 20, w2s.y - 10}, WHITE, "%s", entity->GetPlayerName());

        spdlog::info("player_name {}, player ({},{},{}), distance {}", std::string(entity->GetPlayerName()), w2s.x, w2s.y, 0, localPlayer->GetPlayerPos().Distance(entity->GetPlayerPos()));
    }

    render->Restore();

    return swapBuffers_Original(_hdc);
}

static DWORD __stdcall FakeEntry(HMODULE _hModule) {

    FILE *file{0};
    AllocConsole();
    freopen_s(&file, "CONOUT$", "w", stdout);

    spdlog::info("Test ac_base {:#0x}", moduleBase);

    // MH_Initialize();

    // if (MH_CreateHookApi(L"opengl32.dll", "wglSwapBuffers", (void **)&wglSwapBuffers_Hook, (void **)&swapBuffers_Original) != MH_OK)
    //     spdlog::error("Failed to hook wglSwapBuffers");

    // MH_EnableHook(MH_ALL_HOOKS);

    swapBuffers_Original = (wglSwapBuffers)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    swapBuffers_Original = (wglSwapBuffers)memory::Trampoline((char *)swapBuffers_Original, (char *)wglSwapBuffers_Hook, 5);

    while (!GetAsyncKeyState(VK_END)) {

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    /*  MH_DisableHook(MH_ALL_HOOKS);
      MH_Uninitialize();*/
    VirtualFree(swapBuffers_Original, 0, MEM_RELEASE);

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