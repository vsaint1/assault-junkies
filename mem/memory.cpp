#include "memory.h"


uint8_t* Memory::findSignature(const char* szSignature) noexcept
{
	const auto module = this->getModuleBase();

	using bytes_t = std::vector<std::int32_t>;

	static const auto pattern_to_byte = [](const char* pattern) noexcept -> bytes_t
		{
			bytes_t bytes{};
			const auto start{ const_cast<char*>(pattern) };
			const auto end{ const_cast<char*>(pattern) + strlen(pattern) };

			for (auto current{ start }; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}

			return bytes;
		};

	const auto dosHeader{ reinterpret_cast<PIMAGE_DOS_HEADER>(module) };
	const auto ntHeaders{ reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dosHeader->e_lfanew) };
	const auto textSection{ IMAGE_FIRST_SECTION(ntHeaders) };

	const auto sizeOfImage{ textSection->SizeOfRawData };
	const auto patternBytes{ pattern_to_byte(szSignature) };
	const auto scanBytes{ reinterpret_cast<std::uint8_t*>(module) + textSection->VirtualAddress };

	const auto s{ patternBytes.size() };
	const auto d{ patternBytes.data() };

	MEMORY_BASIC_INFORMATION mbi{ nullptr };
	const std::uint8_t* next_check_address{ nullptr };
	this->m_moduleSize = sizeOfImage;

	for (auto i{ 0ul }; i < sizeOfImage - s; ++i) {
		bool found{ true };
		for (auto j{ 0ul }; j < s; ++j) {
			const auto current_address{ scanBytes + i + j };
			if (current_address >= next_check_address) {
				if (!::VirtualQuery(current_address, &mbi, sizeof(mbi)))
					break;

				if (mbi.Protect == PAGE_NOACCESS) {
					i += reinterpret_cast<std::uintptr_t>(mbi.BaseAddress) + mbi.RegionSize - (reinterpret_cast<std::uintptr_t>(scanBytes) + i);
					i--;
					found = false;
					break;
				}
				else {
					next_check_address = static_cast<std::uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
				}
			}

			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}

		if (found)
			return &scanBytes[i];
	}

	return nullptr;

}

bool Memory::patch(uint8_t* address, char* bytes, unsigned int length) noexcept
{
	LOG("Patching address: %p", address);

	DWORD oldProtect;
	if (!VirtualProtect(reinterpret_cast<void*>(address), length, PAGE_EXECUTE_READWRITE, &oldProtect))
		return false;

	if (memcpy_s(reinterpret_cast<void*>(address), length, bytes, length) != 0)
		return false;

	return VirtualProtect(reinterpret_cast<void*>(address), length, oldProtect, &oldProtect);
}