#pragma once

#include <cstdint>

#include "../../Memory/Memory.h"

#include "CSPlayerController.h"

#define MAX_PLAYERS 64

class GameEntitySystem {
public:
    template <typename T = BaseEntity>
    T* getBaseEntity(int index)
    {
        if (memory->getBaseEntity)
            return (T*)(memory->getBaseEntity(this, index));
        return nullptr;
    }

    template <typename T = BaseEntity>
    T* getBaseEntityFromHandle(unsigned int handle)
    {
       if (memory->getBaseEntityFromHandle)
            return (T*)(memory->getBaseEntityFromHandle(this, handle));
        return nullptr;
    }

    int getHighestEntityIndex()
    {
        int highestIdx = -1;
        if (memory->getHighestEntityIndex)
            memory->getHighestEntityIndex(this, &highestIdx);
        return highestIdx;
    }

    CSPlayerController* getLocalPlayerController()
    {
        for (int i = 1; i <= MAX_PLAYERS; ++i) {
            CSPlayerController* playerController = getBaseEntity<CSPlayerController>(i);
            if (!playerController || !playerController->isPlayerController() || !playerController->isLocalPlayerController())
                continue;
            return playerController;
        }
        return nullptr;
    }
};