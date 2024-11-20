#include "mem/memory.h"
#include "render/opengl_render.h"
#include "sdk/aim/aimbot.h"
#include "sdk/player.h"
#include "sdk/world.h"

typedef int(__stdcall *wglSwapBuffers)(HDC);
wglSwapBuffers swapBuffers_Original = nullptr;

int __stdcall wglSwapBuffers_Hook(HDC _hdc) {

    if (render->GetHdc() != _hdc)
        render->BuildFont();

    render->SetupOrtho();

    globals::vm = *(ViewMatrix *)(globals::moduleBase + offsets::VIEWMATRIX);

    globals::entityList = (uintptr_t *)(globals::moduleBase + offsets::ENTITY_LIST);

    globals::world = (CWorld *)(globals::moduleBase + offsets::LOCAL_PLAYER);

    // spdlog::info(vm.Debug().str());

    if (GetAsyncKeyState(VK_RBUTTON)) {

        auto ent = aimbot::GetBestTarget();

        if (ent != nullptr) {

            auto angleTo = aimbot::CalcAngle(globals::world->localPlayer->GetHeadPos(), ent->GetHeadPos());

            globals::world->localPlayer->SetViewAngles(angleTo);
        }
    }

    for (size_t i = 0; i < (globals::world->playerCount - 1); i++) {

        if (globals::world->localPlayer == nullptr)
            continue;

        if (globals::entityList == nullptr)
            continue;

        uintptr_t *listEntry = (uintptr_t *)(*globals::entityList + (i * sizeof(void *)));

        if (listEntry == nullptr)
            continue;

        CEntity *entity = (CEntity *)(*listEntry);

        if (!entity->IsValid())
            continue;

        if (!entity->IsAlive())
            continue;

        auto headPos = entity->GetHeadPos().WorldToScreen(globals::vm, render->GetWidth(), render->GetHeight());

        auto screenPos = entity->GetPlayerPos().WorldToScreen(globals::vm, render->GetWidth(), render->GetHeight());

        //     auto mode = (uintptr_t *)(globals::moduleBase + offsets::WORLD);

        if (configs::esp::enabled) {

            if (entity->IsEnemy(globals::world->localPlayer)) {

                render->DrawLine({render->GetWidth() / 2, 0}, headPos);

                render->Text({headPos.x, headPos.y - 5}, WHITE, "%s", entity->GetPlayerName());

                render->Text({screenPos.x + 100.f / 2, screenPos.y}, WHITE, "%d", entity->GetHealth());

                render->DrawRect(screenPos, headPos, RED);
            }
        }
    }

    render->Restore();

    return swapBuffers_Original(_hdc);
}

static DWORD __stdcall FakeEntry(HMODULE _hModule) {

    FILE *file{0};
    AllocConsole();
    freopen_s(&file, "CONOUT$", "w", stdout);

    globals::moduleBase = (uintptr_t)GetModuleHandle("ac_client.exe");

    spdlog::info("base ac_client.exe {:#0x}", globals::moduleBase);

    swapBuffers_Original = (wglSwapBuffers)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    swapBuffers_Original = (wglSwapBuffers)memory::Trampoline((char *)swapBuffers_Original, (char *)wglSwapBuffers_Hook, 5);

    while (!GetAsyncKeyState(VK_END)) {

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