#pragma once

#include "../VirtualMethod.h"

class EngineClient {
public:
    bool isConnected()
    {
        return VirtualMethod::getMethod<bool(__thiscall*)(void*)>(this, 31)(this);
    }

    bool isInGame()
    {
        return VirtualMethod::getMethod<bool(__thiscall*)(void*)>(this, 30)(this);
    }
};