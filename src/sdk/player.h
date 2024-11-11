#ifndef PLAYER_H
#define PLAYER_H

#include "math.h"
#include <array>
#include <cstdint>
#include <string.h>

class CWeapon {
  public:
    char name[16];     // 0x0000
    char pad_0010[4];  // 0x0010
    char type[16];     // 0x0014
    char pad_0024[48]; // 0x0024
};                     // Size: 0x0054

class CBaseWeapon {
  public:
    char pad_0000[12];     // 0x0000
    class CWeapon *weapon; // 0x000C
    char pad_0010[44];     // 0x0010
};                         // Size: 0x003C

class CPlayer;

class CEntityList {

  public:
    static std::array<const CPlayer *, 32> GetEntities() {}

  private:
    const std::array<const CPlayer *, 32> entities;
};

 class CPlayer {
   private:
     char pad_0000[4];              // 0x0000
     Vector3 headPos;               // 0x0004
     char pad_0010[24];             // 0x0010
     Vector3 playerPos;             // 0x0028
     char pad_0034[8];              // 0x0034
     Vector2 viewPos;               // 0x003C
     char pad_0044[168];            // 0x0044
     int32_t health;                // 0x00EC
     int32_t armor;                 // 0x00F0
     char pad_00F4[20];             // 0x00F4
     int32_t secondaryClip;         // 0x0108
     char pad_010C[16];             // 0x010C
     int32_t primaryClip;           // 0x011C
     char pad_0120[12];             // 0x0120
     int32_t secondaryAmmo;         // 0x012C
     char pad_0130[16];             // 0x0130
     int32_t primaryAmmo;           // 0x0140
     int32_t grenadeCount;          // 0x0144
     char pad_0148[188];            // 0x0148
     char pad_01[1];                // 0x01
     char name[16];                 // 0x0205
     char pad_0214[336];            // 0x0214
     class CBaseWeapon *baseWeapon; // 0x0364
     char pad_0368[1756];           // 0x0368

   public:
     char *GetPlayerName() {

         if (strlen(name) == 0)
             return (char *)"UNKOWN_PLAYER";

         return name;
     }

     char *GetWeaponName() { return this->baseWeapon->weapon->name; }

     char *GetWeaponType() { return this->baseWeapon->weapon->type; }

     bool IsAlive() { return (this->health > 0 && this->health <= 100); }

     const bool IsValid() noexcept {

       if (this == nullptr)
             return false;

         return true;
     }

     Vector3 GetHeadPos() {

         if (this->headPos.Invalid())
             return {};

         return this->headPos;
     }

     Vector2 GetViewPos() { return this->viewPos; }

     Vector3 GetPlayerPos() { return this->playerPos; }

     int32_t GetHealth() { return this->health; }

     int32_t GetArmor() { return this->armor; }

     int32_t GetSecondaryClip() { return this->secondaryClip; }

     int32_t GetPrimaryClip() { return this->primaryClip; }

     int32_t GetSecondaryAmmo() { return this->secondaryAmmo; }

     int32_t GetPrimaryAmmo() { return this->primaryAmmo; }

     int32_t GetGrenadeCount() { return this->grenadeCount; }

 }; // Size: 0x0A44

#endif // PLAYER_H
