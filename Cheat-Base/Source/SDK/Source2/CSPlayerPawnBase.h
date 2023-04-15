#pragma once

#include "BasePlayerPawn.h"

class CSPlayerPawnBase : public BasePlayerPawn {
public:
    bool isEnemyTo(int team);
};