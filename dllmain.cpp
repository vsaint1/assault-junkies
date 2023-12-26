#include "mem/memory.h"
#include "mem/signature_scanner.h"
#include "cheetos/globals.hpp"
#include "cheetos/sdk/entity.hpp"
#include "cheetos/sdk/hook/hook.hpp"


static unsigned long __stdcall thread(HMODULE hModule) {
	FILE* file;
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", stdout);


	if(Hook::setup())
		LOG("Hooks done!");

	Memory* memory = new Memory(GAME_NAME);


	auto ammo_dec = memory->findSignature("ff 08 8d 44 ?");
	auto health_sub = memory->findSignature("2b f1 29 73");
	auto spread_ret = memory->findSignature("83 EC ? 53 55 8B 6C ? ? 56 57 8B F9");
	auto entity_list = memory->findSignature("a1 ? ? ? ? 8b 34 ? 85 f6 0f 84 ? ? ? ? 80 7e ? ? 0f 85");
	auto local_player = memory->findSignature("8B 0D ? ? ? ? 56 57 8B 3D");
	auto player_count = memory->findSignature("8b 3d ? ? ? ? 33 f6 85 ff 7e ? a1");


	LOG("ammo_dec: %p", ammo_dec);
	LOG("health_sub: %p", health_sub);
	LOG("spread_ret: %p", spread_ret);
	LOG("entity_list: %p", entity_list);
	LOG("local_player: %p", local_player);
	LOG("player_count: %p", player_count);


	EntityList* entityList = memory->read<EntityList*>(memory->getModuleBase() + 0x0191FCC);


	// before patch hook DoDamage function and return if its localplayer
	//memory->patch(health_sub, "\x90\x90", 3);
	memory->patch(ammo_dec, "\x90\x90\x90", 2);
	memory->patch(spread_ret, "\xC2\x08\x00\x83\xEC", 3);

	while (true) {

		
		if (entityList == nullptr)
			entityList = memory->read<EntityList*>(memory->getModuleBase() + 0x0191FCC);

		/*uintptr_t plocalplayer = memory->read<uintptr_t>(memory->getModuleBase() + 0x017E254);

		if (!plocalplayer)
			continue;*/


		int playerCount = memory->read<int>(memory->getModuleBase() + 0x18AC0C);

		if (playerCount > 32)
			continue;

		for (int i = 0; i < playerCount; i++) {



		/*	ViewMatrix viewMatrix = memory->read<ViewMatrix>(memory->getModuleBase() + 0x017DFFC);
			LOG("viewMatrix: %f %f %f", viewMatrix.matrix[0], viewMatrix.matrix[1], viewMatrix.matrix[2]);

			memory->write<int>(plocalplayer + 0xEC, 1234);
			memory->write<int>(plocalplayer + 0x144, 1234);*/

			Entity* entity = entityList->entities[i];

			if (entity == nullptr)
				continue;

			if(entity->isInvalid())
				continue;


			if (!entity->isAlive())
				continue;


			LOG("entity-health %d, %s, %f %f %f", entity->health, entity->normalizedName().c_str(), entity->headPos.x, entity->headPos.y, entity->headPos.z);
		}


		if (GetAsyncKeyState(VK_END))
			break;

		Sleep(1000);
	}

	Hook::shutdown();
	fclose(file);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)thread, hModule, 0, nullptr);
	}

	if (dwReason == DLL_PROCESS_DETACH) {
		//Hook::shutdown();
	}

	return TRUE;
}
