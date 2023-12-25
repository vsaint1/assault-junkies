#pragma once
#include <cstdint>
#include <string>

namespace offsets {
	inline std::ptrdiff_t local_player = 0x017E254;
	inline std::ptrdiff_t entity_list = 0x0191FCC;
	inline std::ptrdiff_t player_count = 0x18AC0C;
	inline std::ptrdiff_t view_matrix = 0x017DFFC;


	inline std::string  ammo_sig = "ff 08 8d 44 ?";
	inline std::string  health_sig = "2b f1 29 73";
	inline std::string  spread_sig = "83 EC ? 53 55 8B 6C ? ? 56 57 8B F9";
	inline std::string  entityList_sig = "a1 ? ? ? ? 8b 34 ? 85 f6 0f 84 ? ? ? ? 80 7e ? ? 0f 85";
	inline std::string  localPlayer_sig = "8B 0D ? ? ? ? 56 57 8B 3D";

} // namespace offsets