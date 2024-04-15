#include "hook/hook.h"
#include "memory/memory.h"

// @todo : remove from the main xd
struct Vector3
{
	float x, y, z;
};

class Entity
{

public:

	char vtable[4];              // 0x0000
	Vector3 head_pos;               // 0x0004
	char pad_0010[24];             // 0x0010
	Vector3 position;              // 0x0028
	Vector3 view_angles;            // 0x0034
	char pad_0040[172];            // 0x0040
	uint32_t health;               // 0x00EC
	int32_t armor;                 // 0x00F0
	char pad_00F4[20];             // 0x00F4
	uint32_t secondary_clip_ammo;    // 0x0108
	char pad_010C[16];             // 0x010C
	uint32_t primary_clip_ammo;      // 0x011C
	char pad_0120[12];             // 0x0120
	uint32_t secondary_current_ammo; // 0x012C
	char pad_0130[16];             // 0x0130
	uint32_t primary_current_ammo;   // 0x0140
	uint32_t grenade;              // 0x0144
	char pad_0148[4];              // 0x0148
	uint32_t knifeDelay;           // 0x014C
	uint32_t secondary_gun_wait_time; // 0x0150
	char pad_0154[16];             // 0x0154
	uint32_t primary_gun_wait_time;   // 0x0164
	char pad_0168[12];             // 0x0168
	uint32_t secondary_shoots_fired; // 0x0174
	char pad_0178[16];             // 0x0178
	uint32_t primary_shoots_fired;   // 0x0188
	char pad_018C[80];             // 0x018C
	int32_t killCount;             // 0x01DC
	char pad_01E0[36];             // 0x01E0
	char player_name[16];                 // 0x0205
	char pad_0208[352];            // 0x0208
	char pad_036C[240];            // 0x036C
	char weapon_type[16];     // 0x0000
	char pad_0004[12]; // 0x0004
	char weapon_name[16];     // 0x0017
	char pad_0014[48]; // 0x0014


}; // Size: 0x045C



static unsigned long __stdcall _thread(HMODULE hModule)
{

	FILE* file{ 0 };
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", __acrt_iob_func(1));

	auto module_base = memory->get_module_base(GAME_NAME);
	auto local_player_sig = memory->find_pattern_offset(GAME_NAME, "8B 0D ? ? ? ? 56 57 8B 3D", 2);

	auto entity_list_sig = memory->find_pattern_offset(GAME_NAME, "A1 ? ? ? ? ? ? ? ? F6 0F 84 5F", 1);
	auto num_player_sig = memory->find_pattern_offset(GAME_NAME, "8B 0D ? ? ? ? 46 3B ? 7C ? 8B 35", 2);

	if (!hook::setup())
		spdlog::warn("Failed while initializing hooks");

	spdlog::info("Initialized");

	uintptr_t* entity_list = (uintptr_t*)(module_base + entity_list_sig);

	while (!GetAsyncKeyState(VK_END))
	{


		if (entity_list == nullptr)
			continue;

		for (size_t i = 0; i < *(int*)num_player_sig; i++)
		{


			uintptr_t* list_entry = (uintptr_t*)(*entity_list + (i * 0x4));

			if (list_entry == nullptr)
				continue;

			Entity* entity = (Entity*)(*list_entry);

			if (entity == nullptr)
				continue;

			if (entity->health > 100)
				continue;

			std::string player_name_str = std::string(entity->player_name);

			if (player_name_str.empty())
				player_name_str = "unknown";

			spdlog::info("player {}, health {}, name {}", (uintptr_t)entity, entity->health, player_name_str);


		}

		Sleep(1000);
	}

	hook::shutdown();

	fclose(file);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, NULL);
}

int __stdcall DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	UNREFERENCED_PARAMETER(reserved);

	if (reason == DLL_PROCESS_ATTACH)
	{

		DisableThreadLibraryCalls(module);

		auto hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)_thread, module, 0, nullptr);

		if (hThread)
			CloseHandle(hThread);
	}

	if (reason == DLL_PROCESS_DETACH)
		return TRUE;

	return TRUE;
}
