#include "aimbot.h"

CEntity *aimbot::GetBestTarget() {

    if (!configs::aimbot::enabled || globals::world->localPlayer == nullptr || globals::entityList == nullptr || globals::world == nullptr) {
        return nullptr;
    }

    float oldDist = FLT_MAX;
    float newDist = 0.0f;

    CEntity *target = nullptr;

    for (size_t i = 0; i < (globals::world->playerCount - 1); i++) {
        uintptr_t *listEntry = (uintptr_t *)(*globals::entityList + (i * sizeof(void *)));
        if (listEntry == nullptr)
            continue;

        CEntity *entity = (CEntity *)(*listEntry);
        if (entity == nullptr) {
            continue;
        }

        if (entity->GetPlayerPos().WorldToScreen(globals::vm, globals::viewport.x, globals::viewport.y).IsZero())
            continue;

        if (!entity->IsValid() || !entity->IsAlive() || !entity->IsEnemy(globals::world->localPlayer)) {
            continue;
        }

        const Vector2 angle = aimbot::CalcAngle(globals::world->localPlayer->GetPlayerPos(), entity->GetPlayerPos());

        newDist = globals::world->localPlayer->GetViewAngles().Distance(angle);

        if (newDist < oldDist) {
            oldDist = newDist;
            target = entity;
        }
    }

    return target;
}

Vector2 aimbot::CalcAngle(Vector3 src, Vector3 dst) {
    Vector2 angle;

    Vector3 delta = dst - src;

    angle.x = -atan2f(delta.x, delta.y) / PI * 180.0f + 180.0f;

    float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);
    angle.y = RAD2DEG(asinf(delta.z / src.Distance(dst)));

    return angle;
}
