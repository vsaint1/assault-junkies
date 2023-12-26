#pragma once
#include "../math/math.hpp"
#include <cstdint>
#include <string>


class EntityList {
public:
	class Entity* entities[32];
};

class Entity {
public:
	char pad_0000[4];             // 0x0000
	Vector3 headPos;              // 0x0004
	char pad_0010[24];            // 0x0010
	Vector3 position;             // 0x0028
	Vector3 viewAngle;            // 0x0034
	char pad_0040[172];           // 0x0040
	uint32_t health;              // 0x00EC
	int32_t armor;                // 0x00F0
	char pad_00F4[20];            // 0x00F4
	int32_t secondaryClipAmmo;    // 0x0108
	char pad_010C[16];            // 0x010C
	int32_t primaryClipAmmo;      // 0x011C
	char pad_0120[12];            // 0x0120
	int32_t secondaryCurrentAmmo; // 0x012C
	char pad_0130[16];            // 0x0130
	int32_t primaryCurrentAmmo;   // 0x0140
	int32_t grenade;              // 0x0144
	char pad_0148[4];             // 0x0148
	int32_t knifeDelay;           // 0x014C
	char pad_0150[140];           // 0x0150
	int32_t killCount;            // 0x01DC
	char pad_01E0[36];            // 0x01E0
	char playerName[200];                // 0x0205
	char pad_0208[352];           // 0x0208
	class Inventory* Inventory;   // 0x0368
	char pad_036C[240];           // 0x036C

	bool isAlive() {
		return this->health > 0 && this->health <= 100;
	}

	std::string normalizedName() {
		if (strlen(this->playerName) == 0)
			return "UnknownPlayer";

		return this->playerName;
	}

	bool isInvalid() {
		return this->position.isZero() || this->viewAngle.isZero() || this->headPos.isZero() || this == nullptr;
	}

};
