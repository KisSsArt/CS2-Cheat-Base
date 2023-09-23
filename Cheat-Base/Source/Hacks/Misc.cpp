#include "Misc.h"

#include "../Config/Config.h"
#include "../Memory/Memory.h"

#include "Source2/GameEntitySystem.h"
#include "Source2/MoveType.h"
#include "Source2/UserCmd.h"
#include "Source2/CSPlayerPawnBase.h"

void Misc::BunnyHop(UserCmd* cmd)
{
    static auto lastJumped = false;
    static auto shouldJump = false;

    if (!config->misc.bunnyHop)
        return;

    auto localEntity = memory->entitySystem->getLocalPlayerController();
    if (!localEntity->pawnIsAlive())
        return;

    CSPlayerPawnBase* pawn = localEntity->getPawn();
    if (!pawn)
        return;

    if (pawn->moveType() == MoveType::MOVETYPE_LADDER)
        return;

    if (!lastJumped && shouldJump) {
        shouldJump = false;
        cmd->buttons |= UserCmd::IN_JUMP;
    }
    else if (cmd->buttons & UserCmd::IN_JUMP) {
        if (pawn->flags() & fl_onground) {
            lastJumped = true;
            shouldJump = true;
        }
        else {
            cmd->buttons &= ~UserCmd::IN_JUMP;
            lastJumped = false;
        }
    }
    else {
        lastJumped = false;
        shouldJump = false;
    }
}
