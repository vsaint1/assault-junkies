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

bool patch(uintptr_t address, char* bytes, unsigned int length) {
	DWORD oldProtect;
	LOG("Patching address: %p", address);
	if (!VirtualProtect(reinterpret_cast<void*>(address), length, PAGE_EXECUTE_READWRITE, &oldProtect))
		return false;

	if (memcpy_s(reinterpret_cast<void*>(address), length, bytes, length) != 0)
		return false;

	return VirtualProtect(reinterpret_cast<void*>(address), length, oldProtect, &oldProtect);
}


static unsigned long __stdcall thread(HMODULE hModule) {
	FILE* file;
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", stdout);

	LOG("DLL attached!");

	setupHooks();

	Memory* memory = new Memory(GAME_NAME);

	Signature health_sub{ "2b f1 29 73" }; 
	Signature ammo_dec{ "ff 08 8d 44 ?" }; 
	Signature spread_ret{ "83 EC ? 53 55 8B 6C ? ? 56 57 8B F9" };
	Signature signatures[3] = { health_sub, ammo_dec, spread_ret };
	SignatureScanner* scanner = new SignatureScanner(memory->getModuleBase(), memory->getModuleSize());
	scanner->scanAll(signatures, sizeof(signatures) / sizeof(Signature));


	patch(signatures[0].address, "\x90\x90", 3);
	patch(0x4C73EF, "\x90\x90\x90", 2);
	patch(signatures[2].address, "\xC2\x08\x00\x83\xEC", 3);

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
