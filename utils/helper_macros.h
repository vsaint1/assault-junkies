#pragma once

#define GAME_NAME "ac_client.exe"
#define GAME_NAME_W L"ac_client.exe"
#define MODULE_SIZE 0x001B9000
#define MODULE_NAME "ac_client.exe"

#ifdef _DEBUG
#define LOG(fmt, ...)\
{\
const auto filename = strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__;\
printf("[%s:%d] - " fmt "\n", filename, __LINE__, __VA_ARGS__);\
}
#else
#define LOG(fmt, ...)\
{\
printf("[assault-junkies] - " fmt "\n", __VA_ARGS__);\
}
#endif

#ifdef _WIN32
#define ASM_NOOP 0x90
#define ASM_JMP 0xE9
#define ASM_CALL 0xE8
#define ASM_RET 0xC3
#define ASM_MOV 0xB8
#else
#error "Unsupported architecture"
#endif
