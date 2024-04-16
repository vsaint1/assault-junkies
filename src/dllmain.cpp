#include "features/sdk/entity.h"
#include "hook/hook.h"
#include "memory/memory.h"




static unsigned long __stdcall _thread(HMODULE hModule) {

  FILE *file{0};
  AllocConsole();
  freopen_s(&file, "CONOUT$", "w", __acrt_iob_func(1));

  auto module_base = memory->get_module_base(GAME_NAME);
  auto local_player_sig = memory->find_pattern_offset(GAME_NAME, "8B 0D ? ? ? ? 56 57 8B 3D", 2);

  auto entity_list_sig = memory->find_pattern_offset(GAME_NAME, "A1 ? ? ? ? ? ? ? ? F6 0F 84 5F", 1);
  auto num_player_sig = memory->find_pattern_offset(GAME_NAME, "8B 0D ? ? ? ? 46 3B ? 7C ? 8B 35", 2);
  auto game_mode_sig = memory->find_pattern_offset(GAME_NAME, "89 15 ? ? ? ? 53", 2);


  if (!hook::setup())
   spdlog::warn("Failed while initializing hooks");

  spdlog::info("Initialized");

 uintptr_t * entity_list = (uintptr_t *)(entity_list_sig);

  Entity* local_player = *(Entity**)(local_player_sig);


  while (!GetAsyncKeyState(VK_END)) {

    if (local_player == nullptr)
      continue;

    local_player->health = 1337;
    local_player->primary_current_ammo = 999;
    local_player->grenade = 999;


    if (entity_list == nullptr)
      continue;

    for (size_t i = 0; i < *(int *)num_player_sig; i++) {

      uintptr_t *list_entry = (uintptr_t *)(*entity_list + (i * 0x4));

      if (list_entry == nullptr)
        continue;

      Entity *entity = (Entity *)(*list_entry);

      if (entity == nullptr)
        continue;

      if (entity->health > 100)
        continue;

      std::string player_name_str = std::string(entity->player_name);

      if (player_name_str.empty())
        player_name_str = "unknown_player";

      float distance = local_player->position.distance(entity->position);
      spdlog::info("player {}, health {}, player_name {}, distance {}, game_mode {}", (uintptr_t)entity, entity->health, player_name_str, distance, entity->team_are_enemies( *(int *)game_mode_sig) ? "ffa" : "team");
    }

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
