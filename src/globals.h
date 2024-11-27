#ifndef GLOBALS_H
#define GLOBALS_H

#include "sdk/math.h"
#include "sdk/player.h"
#include <cstdint>
#include <map>
class CWorld;

namespace globals{

inline uintptr_t moduleBase;
inline uintptr_t *entityList = nullptr;
inline CWorld *world = nullptr;
inline ViewMatrix vm;
inline Vector2 viewport = { 0, 0 };


}

namespace configs {

namespace aimbot {
inline bool enabled = true;
inline float fov = 30.0f;
inline float smooth = 20.0f;
inline char key = VK_RBUTTON;
inline int bone_id = 0;
inline std::map<int, std::string> bones = { {0, "head"}, {1, "chest"}, {2, "stomach"}, {3, "pelvis"} };
} // namespace aimbot
namespace esp {
inline bool enabled = true;
inline bool team = false;
} // namespace esp

} // namespace configs



namespace offsets {
constexpr uintptr_t LOCAL_PLAYER = 0x18AC00;
constexpr uintptr_t PLAYER_COUNT = 0x18AC0C;
constexpr uintptr_t GAME_MODE = 0x18ABF8;
constexpr uintptr_t VIEWMATRIX = 0x17DFD0;
constexpr uintptr_t ENTITY_LIST = 0x191FCC;
} // namespace offsets

#endif // GLOBALS_H
