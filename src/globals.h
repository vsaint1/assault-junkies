#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstdint>

namespace configs
{
    
} // namespace configs


namespace offsets
{
    constexpr uintptr_t LOCAL_PLAYER = 0x18AC00;
    constexpr uintptr_t PLAYER_COUNT = 0x18AC0C;
    constexpr uintptr_t WORLD = 0x18ABF8;
    constexpr uintptr_t VIEWMATRIX = 0x17DFD0;
    constexpr uintptr_t ENTITY_LIST = 0x191FCC;
    } // namespace offsets


#endif // GLOBALS_H
