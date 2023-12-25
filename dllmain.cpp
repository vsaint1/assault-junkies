#include <minhook/MinHook.h>
#include "mem/memory.h"
#include "mem/signature_scanner.h"
#include "cheetos/globals.hpp"

typedef BOOL(__stdcall* twglSwapBuffers)(HDC hDc);
typedef void(__stdcall* sub_4CCA80)(float* a1, int a2);

twglSwapBuffers originalSwapBuffers = nullptr;

typedef void(__fastcall* sub_4EA850)(unsigned int a1, const char* a2, ...);

sub_4EA850 originalPrint = nullptr;
struct ViewMatrix {
	float matrix[16];
};

struct Vector3 {
	float x, y, z;
};

class Entity {
public:
	char pad_0000[4];             // 0x0000
	Vector3 headPos;              // 0x0004
	char pad_0010[24];            // 0x0010
	Vector3 position;             // 0x0028
	Vector3 viewAngle;            // 0x0034
	char pad_0040[172];           // 0x0040
	uint32_t health;              // 0x00EC
	int32_t armor;                // 0x00F0
	char pad_00F4[20];            // 0x00F4
	int32_t secondaryClipAmmo;    // 0x0108
	char pad_010C[16];            // 0x010C
	int32_t primaryClipAmmo;      // 0x011C
	char pad_0120[12];            // 0x0120
	int32_t secondaryCurrentAmmo; // 0x012C
	char pad_0130[16];            // 0x0130
	int32_t primaryCurrentAmmo;   // 0x0140
	int32_t grenade;              // 0x0144
	char pad_0148[4];             // 0x0148
	int32_t knifeDelay;           // 0x014C
	char pad_0150[140];           // 0x0150
	int32_t killCount;            // 0x01DC
	char pad_01E0[36];            // 0x01E0
	wchar_t* playerName[16];                // 0x0205
	char pad_0208[352];           // 0x0208
	class Inventory* Inventory;   // 0x0368
	char pad_036C[240];           // 0x036C
	ViewMatrix viewMatrix;       // 0x045C

	bool isAlive() {
		return this->health > 0 && this->health <= 100;
	}

};
class EntityList {
public:
	class Entity* entities[32];
};

int __stdcall hkSwapBuffers(HDC hDc) {
	//LOG("Hooked opengl!wglSwapBuffers");
	return originalSwapBuffers(hDc);
}

void __fastcall hkPrintText(unsigned int a1, const char* a2, ...) {
	LOG("%d %s", a1, a2);
}

bool setupHooks() {
	if (MH_Initialize() != MH_OK)
		return false;

	MH_STATUS openglHook = MH_CreateHookApi(L"opengl32", "wglSwapBuffers", &hkSwapBuffers,
		reinterpret_cast<void**>(&originalSwapBuffers));

	MH_STATUS test = MH_CreateHook(reinterpret_cast<void*>(0x4EA850), &hkPrintText,
		reinterpret_cast<void**>(&originalPrint));

	if (test != MH_OK)
		return false;

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


static unsigned long __stdcall thread(HMODULE hModule) {
	FILE* file;
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", stdout);

	LOG("DLL attached!");

	setupHooks();

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


		int playerCount = memory->read<int>(memory->getModuleBase() + 0x18AC0C);

		if (playerCount > 32)
			continue;

		for (int i = 0; i < playerCount; i++) {

			uintptr_t plocalplayer = memory->read<uintptr_t>(memory->getModuleBase() + 0x017E254);

			if (!plocalplayer)
				continue;

			ViewMatrix viewMatrix = memory->read<ViewMatrix>(memory->getModuleBase() + 0x017DFFC);
			LOG("viewMatrix: %f %f %f", viewMatrix.matrix[0], viewMatrix.matrix[1], viewMatrix.matrix[2]);
		

			Entity* entity = entityList->entities[i];

			if (entity == nullptr)
				continue;


			if (!entity->isAlive())
				continue;


			LOG("entity: %p, %d, %s, %f %f %f", entity,entity->health, entity->playerName, entity->headPos.x, entity->headPos.y, entity->headPos.z);
		}


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
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)thread, hModule, 0, nullptr);
	}

	if (dwReason == DLL_PROCESS_DETACH) {
		//shutdown();
	}

	return TRUE;
}
