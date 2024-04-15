#pragma once
#include "../utils/macros.h"
#include <intrin.h>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <winternl.h>

class Memory {
public:
  uintptr_t get_module_base(const char *module_name) {

    // @todo: implement 4fun
    auto peb = (PEB *)__readfsdword(0x30);

    if (peb == nullptr)
      return 0;
	
    return (uintptr_t)GetModuleHandleA(module_name);
  }

  uint8_t *find_pattern(const char *module_name, const char *sig, unsigned int byte_offset) noexcept;

  uint32_t find_pattern_offset(const char *module_name, const char *sig, unsigned int byte_offset = 0) {
    const auto signature = this->find_pattern(module_name, sig, byte_offset);

    return *(uint32_t *)signature;
  }

  bool patch(uint8_t *address, char *bytes, unsigned int length) noexcept;
};

inline auto memory = std::make_unique<Memory>();
