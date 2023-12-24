#include <minhook/MinHook.h>
#include "mem/memory.h"
#include "mem/signature_scanner.h"

typedef BOOL(__stdcall* twglSwapBuffers)(HDC hDc);

twglSwapBuffers originalSwapBuffers = nullptr;

int __stdcall hkSwapBuffers(HDC hDc) {
	//LOG("Hooked opengl!wglSwapBuffers");
	return originalSwapBuffers(hDc);
}


bool setupHooks() {
	if (MH_Initialize() != MH_OK)
		return false;

	MH_STATUS openglHook = MH_CreateHookApi(L"opengl32", "wglSwapBuffers", &hkSwapBuffers,
		reinterpret_cast<void**>(&originalSwapBuffers));

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

	LOG("ammo_dec: %p", ammo_dec);
	LOG("health_sub: %p", health_sub);
	LOG("spread_ret: %p", spread_ret);

	memory->patch(health_sub, "\x90\x90", 3);
	memory->patch(ammo_dec, "\x90\x90\x90", 2);
	memory->patch(spread_ret, "\xC2\x08\x00\x83\xEC", 3);

	while (true) {
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
