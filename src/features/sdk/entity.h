#pragma once
#include "../math.h"
#include <cstdint>

class Entity {

public:
  char vftable[4];                  // 0x0000
  Vector3 head_pos;                 // 0x0004
  char pad_0010[24];                // 0x0010
  Vector3 position;                 // 0x0028
  Vector3 view_angles;              // 0x0034
  char pad_0040[172];               // 0x0040
  uint32_t health;                  // 0x00EC
  int32_t armor;                    // 0x00F0
  char pad_00F4[20];                // 0x00F4
  uint32_t secondary_clip_ammo;     // 0x0108
  char pad_010C[16];                // 0x010C
  uint32_t primary_clip_ammo;       // 0x011C
  char pad_0120[12];                // 0x0120
  uint32_t secondary_current_ammo;  // 0x012C
  char pad_0130[16];                // 0x0130
  uint32_t primary_current_ammo;    // 0x0140
  uint32_t grenade;                 // 0x0144
  char pad_0148[4];                 // 0x0148
  uint32_t knifeDelay;              // 0x014C
  uint32_t secondary_gun_wait_time; // 0x0150
  char pad_0154[16];                // 0x0154
  uint32_t primary_gun_wait_time;   // 0x0164
  char pad_0168[12];                // 0x0168
  uint32_t secondary_shoots_fired;  // 0x0174
  char pad_0178[16];                // 0x0178
  uint32_t primary_shoots_fired;    // 0x0188
  char pad_018C[80];                // 0x018C
  int32_t killCount;                // 0x01DC
  char pad_01E0[36];                // 0x01E0
  char player_name[16];             // 0x0205
  char pad_0208[352];               // 0x0208
  char pad_036C[240];               // 0x036C
  char weapon_type[16];             // 0x0000
  char pad_0004[12];                // 0x0004
  char weapon_name[16];             // 0x0017
  char pad_0014[48];                // 0x0014

  /*
  7 -  BTD
  8 -  DM
  12 - 1S1K
  18 - PFrenzy
  19 - LSstanding
  20 - Tsurvivor
  21 - TSTK

  */
  bool team_are_enemies(int game_mode) {

    switch (game_mode) {
    case 7:
      return false;
    case 8:
      return true;
    case 12:
      return true;
    case 18:
      return true;
    case 19:
      return true;
    case 20:
      return false;
    case 21:
      return false;
    default:
      return false;
    }
  }
}; // Size: 0x045C