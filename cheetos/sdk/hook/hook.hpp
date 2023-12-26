#pragma once
#include <minhook/MinHook.h>
#include <stdio.h>
#include "../utils/helper_macros.h"

typedef BOOL(__stdcall* twglSwapBuffers)(HDC hDc);

typedef void(__fastcall* sub_4EA850)(unsigned int a1, const char* a2, ...);


class Hook {

public:
	Hook() = default;

	~Hook() = default;

	Hook(const Hook&) = delete;

	Hook& operator=(const Hook&) = delete;

	static bool setup();

	static void shutdown();

	static BOOL __stdcall hkSwapBuffers(HDC hDc);

	static void __fastcall hkConsolePrint(unsigned int a1, const char* a2, ...);


};

