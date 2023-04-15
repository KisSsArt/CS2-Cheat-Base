#pragma once

#include "../VirtualMethod.h"
#include "../Vector.h"

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

    void getScreenSize(int& w, int& h)
    {
        using getScreenSize_t = void(__thiscall*)(void*, int&, int&);
        VirtualMethod::getMethod<getScreenSize_t>(this, 48)(this, w, h);
    }

    Vector getScreenSize()
    {
        int w, h;
        getScreenSize(w, h);
        return { static_cast<float>(w), static_cast<float>(h), 0.0f };
    }
};