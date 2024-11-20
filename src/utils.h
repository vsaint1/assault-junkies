#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>
#include <cstdio>
#include <thread>
#include <MinHook.h>
#include "globals.h"

#define MODULE_BASE "ac_client.exe"
#define MODULE_BASE_W L"ac_client.exe"

#define JMP 0xE9
#define NOP 0x90

#ifdef _DEBUG
#include <spdlog/spdlog.h>
#else 
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#include <spdlog/spdlog.h>
#endif

#endif // UTILS_H
