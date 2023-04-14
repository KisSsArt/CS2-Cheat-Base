#pragma once

#include <cstdint>

class InputSystem {
public:
    bool& IsRelativeMouseMode() 
    {
        return *reinterpret_cast<bool*>((uintptr_t)(this) + 0x4F);
    }

    void* GetSDLWindow() 
    {
        return *reinterpret_cast<void**>((uintptr_t)(this) + 0x2670);
    }

    void enableInput(bool state);
};
