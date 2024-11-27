#ifndef AIMBOT_H
#define AIMBOT_H

#include "../player.h"
#include "../../globals.h"

#define MAX_PITCH 180.0f

namespace aimbot{

    CEntity* GetBestTarget();

    Vector2 CalcAngle(Vector3 src, Vector3 dst);

};

#endif // AIMBOT_H
