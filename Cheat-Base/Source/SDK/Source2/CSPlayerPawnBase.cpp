#include "CSPlayerPawnBase.h"

#include "CSPlayerController.h"

bool CSPlayerPawnBase::isEnemyTo(int team)
{
    CSPlayerController* playerController = controller().get<CSPlayerController>();
    if (!playerController)
        return true;

    if (playerController->isLocalPlayerController())
        return false;

    return this->team() != team;
}