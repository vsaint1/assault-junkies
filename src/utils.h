#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>
#include <cstdio>
#include <thread>
#include <MinHook.h>
#include "globals.h"

#define GAME_NAME "ac_client.exe"
#define GAME_NAME_W L"ac_client.exe"

#ifdef _DEBUG
#include <spdlog/spdlog.h>
#else 
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#include <spdlog/spdlog.h>
#endif

#endif // UTILS_H
