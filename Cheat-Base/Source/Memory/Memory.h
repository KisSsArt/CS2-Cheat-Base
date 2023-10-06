#pragma once

#include <memory>

#include "ScanUtils/stb.h"
#include "ScanUtils/Module.h"

#define SIGNATURE(sig) stb::simple_conversion::build<stb::fixed_string{ sig }>::value

struct GlobalVars;
struct Vector;
class Input;
class GameEntitySystem;

class Memory {
public:
    Memory() noexcept;
    CModule clientModule;
    CModule sdl3;

    GlobalVars* globalVars;
    Input* input;
    GameEntitySystem* entitySystem;
    bool(__fastcall* worldToScreen)(Vector&, Vector&);

    std::uintptr_t createMove;
    std::uintptr_t levelInit;
    std::uintptr_t frameStage;
    std::uintptr_t mouseInputEnabled;

    void* (__thiscall* getBaseEntity)(void*, int);
    void* (__thiscall* getBaseEntityFromHandle)(void*, unsigned int);
    int(__thiscall* getHighestEntityIndex)(void*, int*);

    int(__stdcall* SDL_SetRelativeMouseMode)(int);
    int(__stdcall* SDL_SetWindowGrab)(void*, int);
    int(__stdcall* SDL_WarpMouseInWindow)(void*, int, int);
};

inline std::unique_ptr<Memory> memory;