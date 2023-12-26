#include "hook.hpp"

twglSwapBuffers originalSwapBuffers = nullptr;

sub_4EA850 originalConsolePrint = nullptr;

bool Hook::setup() {
	if (MH_Initialize() != MH_OK)
		return false;

	MH_STATUS openglHook = MH_CreateHookApi(L"opengl32", "wglSwapBuffers", &Hook::hkSwapBuffers,
		reinterpret_cast<void**>(&originalSwapBuffers));

	MH_STATUS consoleHook = MH_CreateHook(reinterpret_cast<void*>(0x4EA850), &Hook::hkConsolePrint,
		reinterpret_cast<void**>(&originalConsolePrint));

	if (consoleHook != MH_OK)
		return false;

	if (openglHook != MH_OK)
		return false;

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		return false;

	return true;
}

int __stdcall Hook::hkSwapBuffers(HDC hDc) {
	//LOG("Hooked opengl!wglSwapBuffers");
	return originalSwapBuffers(hDc);
}

void __fastcall Hook::hkConsolePrint(unsigned int a1, const char* a2, ...) {
	LOG("%d %s", a1, a2);
	originalConsolePrint(a1, a2);
}

void Hook::shutdown() {
	MH_DisableHook(MH_ALL_HOOKS);

	MH_Uninitialize();
}