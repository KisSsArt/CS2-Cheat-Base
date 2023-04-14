#pragma once

#include <cstdint>

class GameEntitySystem;

class GameResourceService {
public:
    GameEntitySystem* getGameEntitySystem()
    {
        return *reinterpret_cast<GameEntitySystem**>((uintptr_t)(this) + 0x58);
    }
};