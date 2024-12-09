#include "mem/memory.h"
#include "render/opengl_render.h"
#include "sdk/aim/aimbot.h"
#include "sdk/player.h"
#include "sdk/world.h"

typedef int(__stdcall *wglSwapBuffers)(HDC);
wglSwapBuffers swapBuffers_Original = nullptr;

int __stdcall wglSwapBuffers_Hook(HDC _hdc) {

    globals::viewport = {render->GetWidth(), render->GetHeight()};

    if (render->GetHdc() != _hdc)
        render->BuildFont();

    render->SetupOrtho();

    globals::vm = *(ViewMatrix *)(globals::moduleBase + offsets::VIEWMATRIX);

    globals::entityList = (uintptr_t *)(globals::moduleBase + offsets::ENTITY_LIST);

    globals::world = (CWorld *)(globals::moduleBase + offsets::LOCAL_PLAYER);

    // spdlog::info(vm.Debug().str());

    globals::world->localPlayer->GetWeapon()->data->damage = 256;
    globals::world->localPlayer->GetWeapon()->data->delay = 0;
    globals::world->localPlayer->GetWeapon()->data->reload = 0;
    globals::world->localPlayer->GetWeapon()->data->spread = 0;
    globals::world->localPlayer->GetWeapon()->data->impulse = 0;
    globals::world->localPlayer->GetWeapon()->data->recoilX = 0;
    globals::world->localPlayer->GetWeapon()->data->recoilY = 0;

    render->DrawCircle({globals::viewport.x / 2, globals::viewport.y / 2}, configs::aimbot::fov * 2, WHITE);

    if (GetAsyncKeyState(configs::aimbot::key)) {
        auto ent = aimbot::GetBestTarget();

        if (ent != nullptr) {

            auto angleTo = aimbot::CalcAngle(globals::world->localPlayer->GetPlayerPos(), ent->GetPlayerPos());

            if (configs::aimbot::bones[configs::aimbot::bone_id].compare("head") == 0)
                angleTo = aimbot::CalcAngle(globals::world->localPlayer->GetHeadPos(), ent->GetHeadPos());

            auto currentAngle = globals::world->localPlayer->GetViewAngles();

            auto head2D = ent->GetHeadPos().WorldToScreen(globals::vm, globals::viewport.x, globals::viewport.y);

            auto dist = abs(head2D.Distance({render->GetWidth() / 2, render->GetHeight() / 2}));

            if (dist <= configs::aimbot::fov) {
                if (configs::aimbot::smooth < 1.f) {
                    globals::world->localPlayer->SetViewAngles(angleTo);

                } else {
                    currentAngle.x += (angleTo.x - currentAngle.x) / configs::aimbot::smooth;
                    currentAngle.y += (angleTo.y - currentAngle.y) / configs::aimbot::smooth;
                    globals::world->localPlayer->SetViewAngles(currentAngle);
                }
            }
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

    MH_Initialize();
    MH_CreateHookApi(L"opengl32.dll", "wglSwapBuffers", (void *)wglSwapBuffers_Hook, (void **)&swapBuffers_Original);

    MH_EnableHook(MH_ALL_HOOKS);

    // swapBuffers_Original = (wglSwapBuffers)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    // swapBuffers_Original = (wglSwapBuffers)memory::Trampoline((char *)swapBuffers_Original, (char *)wglSwapBuffers_Hook, 5);

    while (!GetAsyncKeyState(VK_END)) {

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

    if (ul_reason_for_call == DLL_PROCESS_DETACH) {

        return TRUE;
    }

    return TRUE;
}