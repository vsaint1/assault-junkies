#ifndef WORLD_H
#define WORLD_H
#include <cstdint>

class CWorld {

  public:
    static bool TeamAreEnemies(int32_t _mode) {
        if (_mode == 8 || _mode == 12 || _mode == 18 || _mode == 19)
            return true;

        return false;
    }

    /*
    7 -  BTD
    8 -  DM
    12 - 1S1K
    18 - PFrenzy
    19 - LSstanding
    20 - Tsurvivor
    21 - TSTK
    */
    static const char *GetGameModeName(int32_t _mode) {
        switch (_mode) {
        case 7:
            return "Team Death Match";
        case 8:
            return "Death Match";
        case 12:
            return "1 Shoot 1 Kill";
        case 18:
            return "Pistol Frenzy";
        case 19:
            return "Last Swiss Standing";
        case 20:
            return "Team Survivor";
        case 21:
            return "Team 1 Shoot 1 Kill";
        default:
            return "UNKNOWN_GAME_MODE";
        }
    }
};

#endif // WORLD_H
