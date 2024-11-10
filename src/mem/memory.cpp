#include "memory.h"

namespace memory {

bool Detour(char *src, char *dest, size_t len) {

    if (len < 5)
        return false;

    DWORD oldProtection;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtection);

    *src = JMP;

    uintptr_t relAddress = dest - src - 5;

    *(uintptr_t *)(src + 1) = relAddress;

    if (len - 5 > 0)
        memset(src, NOP, len - 5);

    VirtualProtect(src, len, oldProtection, &oldProtection);

    return true;
}

char *Trampoline(char *src, char *dest, size_t len) {

    char *trampAddress = (char *)VirtualAlloc(NULL, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    memcpy_s(trampAddress, len, src, len);

    uintptr_t relAddress = src - trampAddress - 5;

    *(trampAddress + len) = JMP;

    *(uintptr_t *)(trampAddress + len + 1) = relAddress;

    Detour(src, dest, len);

    return trampAddress;
}

uintptr_t ResolveAddress(uintptr_t baseAddress, const std::vector<uintptr_t> &offsets) {
    uintptr_t address = baseAddress;

    for (auto &offset : offsets) {

        address = *(uintptr_t *)address;
        address += offset;
    }

    return address;
}
uint8_t *FindPattern(const char *moduleName, const char *sig, unsigned int byteOffset) {

    const auto module = GetModuleHandle(moduleName);

    static const auto pattern_to_byte = [](const char *pattern) noexcept -> std::vector<std::int32_t> {
        std::vector<std::int32_t> bytes{};
        const auto start{const_cast<char *>(pattern)};
        const auto end{const_cast<char *>(pattern) + strlen(pattern)};

        for (auto current{start}; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            } else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }

        return bytes;
    };

    const auto dos_header{reinterpret_cast<PIMAGE_DOS_HEADER>(module)};
    const auto nt_headers{reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t *>(module) + dos_header->e_lfanew)};
    const auto text_section{IMAGE_FIRST_SECTION(nt_headers)};

    const auto size_of_image{text_section->SizeOfRawData};
    const auto aob{pattern_to_byte(sig)};
    const auto scanBytes{reinterpret_cast<std::uint8_t *>(module) + text_section->VirtualAddress};

    const auto size{aob.size()};
    const auto data{aob.data()};

    MEMORY_BASIC_INFORMATION mbi{nullptr};
    const std::uint8_t *next_check_address{nullptr};
    auto module_size = nt_headers->OptionalHeader.SizeOfImage;

    for (auto i{0ul}; i < size_of_image - size; ++i) {
        bool found{true};
        for (auto j{0ul}; j < size; ++j) {
            const auto current_address{scanBytes + i + j};
            if (current_address >= next_check_address) {
                if (!VirtualQuery(current_address, &mbi, sizeof(mbi)))
                    break;

                if (mbi.Protect == PAGE_NOACCESS) {
                    i += reinterpret_cast<std::uintptr_t>(mbi.BaseAddress) + mbi.RegionSize - (reinterpret_cast<std::uintptr_t>(scanBytes) + i);
                    i--;
                    found = false;
                    break;
                } else {
                    next_check_address = static_cast<std::uint8_t *>(mbi.BaseAddress) + mbi.RegionSize;
                }
            }

            if (scanBytes[i + j] != data[j] && data[j] != -1) {
                found = false;
                break;
            }
        }

        if (found)
            return &scanBytes[i] + byteOffset;
    }

    return nullptr;
}

uintptr_t FindPatternOffset(const char *moduleName, const char *sig, unsigned int byteOffset) {
    const auto res = memory::FindPattern(moduleName, sig, byteOffset);

    return *(uintptr_t *)res;
}

} // namespace memory
