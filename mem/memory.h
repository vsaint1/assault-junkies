#pragma once

#include <cstdint>

typedef struct _MODULEINFO {
    LPVOID lpBaseOfDll;
    DWORD SizeOfImage;
    LPVOID EntryPoint;
} MODULEINFO, *LPMODULEINFO;

class Memory {
    int pid;
    uintptr_t module;
    unsigned int moduleSize = 0; // TODO: get dynamically from module

public:
    explicit Memory(LPCSTR moduleName) {
        this->module = reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName));
        this->pid = static_cast<int>(GetCurrentProcessId());
    }

    Memory(const Memory&) = delete;

    Memory& operator=(const Memory&) = delete;


    unsigned int getModuleSize() {
        return this->moduleSize;
    }

    uintptr_t getModuleBase() {
        return this->module;
    }

    int getProcessId() {
        return this->pid;
    }

    template<typename T>
    T read(uintptr_t address) {
        return *reinterpret_cast<T *>(address);
    }

    template<typename T>
    void write(uintptr_t address, T value) {
        *reinterpret_cast<T *>(address) = value;
    }
};
