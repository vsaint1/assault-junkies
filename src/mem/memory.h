#ifndef MEMORY_H
#define MEMORY_H

#include "../utils.h"

namespace memory {

bool Detour(char *src, char *dest, size_t len);

char *Trampoline(char *, char *dest, size_t len);

template <typename T> T Read(uintptr_t address) { return *(T *)address; }

template <typename T> void Write(uintptr_t address, const T &value) { *(T *)address = value; }

uintptr_t ResolveAddress(uintptr_t baseAddress, const std::vector<uintptr_t> &offsets);

uint8_t *FindPattern(const char *moduleName, const char *sig, unsigned int byteOffset);

uintptr_t FindPatternOffset(const char *moduleName, const char *sig, unsigned int byteOffset = 0u);

} // namespace memory

#endif // MEMORY_H
