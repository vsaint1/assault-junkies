#ifndef PLAYER_H
#define PLAYER_H

#include "../utils.h"
#include "math.h"
#include <array>
#include <cstdint>
#include <string.h>

class CWeaponData {
  public:
    char name[23];     // 0x0000
    char type[41];     // 0x0017
    int16_t unk;       // 0x0040
    int16_t unk1;      // 0x0042
    int16_t unk2;      // 0x0044
    int16_t reload;    // 0x0046
    int16_t delay;     // 0x0048
    int16_t damage;    // 0x004A
    int16_t unk3;      // 0x004C
    int16_t unk4;      // 0x004E
    int16_t unk5;      // 0x0050
    int16_t spread;    // 0x0052
    int16_t impulse;   // 0x0054
    int16_t maxReload; // 0x0056
    int16_t weaponX;   // 0x0058
    int16_t weaponY;   // 0x005A
    int16_t weaponZ;   // 0x005C
    int16_t recoilX;   // 0x005E
    int16_t recoilY;   // 0x0060
    int16_t unk6;      // 0x0062
    int16_t unk7;      // 0x0064
    int16_t unk8;      // 0x0066
}; // Size: 0x0068

class CWeapon {
  public:
    char pad_0000[12];       // 0x0000
    class CWeaponData *data; // 0x000C
    char pad_0010[52];       // 0x0010
}; // Size: 0x0044

class CEntity {
  public:
    void *vTable;          // 0x0000
    Vector3 headPos;       // 0x0004
    char pad_0010[24];     // 0x0010
    Vector3 playerPos;     // 0x0028
    Vector2 viewAngles;    // 0x0034
    char pad_003C[12];     // 0x003C
    int32_t jumpHeight;    // 0x0048
    char pad_004C[16];     // 0x004C
    int32_t onGround;      // 0x005C
    char pad_0060[4];      // 0x0060
    int32_t inAir;         // 0x0064
    char pad_0068[132];    // 0x0068
    int32_t health;        // 0x00EC
    int32_t armor;         // 0x00F0
    char pad_00F4[268];    // 0x00F4
    char name[16];         // 0x0200
    char pad_0210[252];    // 0x0210
    int32_t team;          // 0x030C
    char pad_0310[8];      // 0x0310
    int32_t unkTeam;       // 0x0318
    char pad_031C[72];     // 0x031C
    class CWeapon *weapon; // 0x0364
    char pad_0368[60];     // 0x0368

    char *GetPlayerName() {
        if (strlen(name) > 0)
            return (char *)name;

        return (char *)"UNKNOWN_PLAYER";
    }

    void SetViewAngles(Vector2 angles) { viewAngles = angles; }

    char *GetWeaponName() {
        if (strlen(weapon->data->name) > 0)
            return (char *)weapon->data->name;

        return (char *)"UNKNOWN_WEAPON";
    }

    char *GetWeaponType() {
        if (strlen(weapon->data->type) > 0)
            return (char *)weapon->data->type;

        return (char *)"UNKNOWN_TYPE";
    }

    CWeapon *GetWeapon() { return weapon; }

    bool IsAlive() { return health > 0 && health <= 100; }

    bool IsEnemy(CEntity *localPlayer) {
        if (this->team == localPlayer->team)
            return false;

        return true;
    }

    Vector3 GetHeadPos() { return headPos; }

    Vector3 GetPlayerPos() { return playerPos; }

    Vector2 GetViewAngles() { return viewAngles; }

    bool IsValid() {
        if (this != nullptr)
            return true;

        return false;
    }
}; // Size: 0x03A4

class CWorld {
  public:
    class CEntity *localPlayer; // 0x0000
    char pad_0004[8];           // 0x0004
    int32_t playerCount;        // 0x000C
    char pad_0010[48];          // 0x0010
};

#endif // PLAYER_H
