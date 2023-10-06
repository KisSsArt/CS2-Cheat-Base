#include "Memory.h"

#include "../Interfaces.h"
#include "../Definitions.h"

#include "Source2/GameResourceService.h"

#include "Utils.h"

Memory::Memory() noexcept
{
    clientModule.init(CLIENTDLL);
    sdl3.init(SDL3DLL);

    entitySystem = interfaces->gameResourceService->getGameEntitySystem();

    globalVars = clientModule.FindPattern(SIGNATURE("48 8B 05 ? ? ? ? 48 8B D8 80 78 3D 00 75 1D 80 78 3C 00 75")).toAbsolute(3, 7).get<GlobalVars*>();
    input = clientModule.FindPattern(SIGNATURE("48 8B 0D ? ? ? ? 48 8B 01 FF 50 18 8B DF 66 0F 1F 44 00 ? 48 8B 0D")).toAbsolute(3, 0).get<Input*>();
    worldToScreen = clientModule.FindPattern(SIGNATURE("E8 ? ? ? ? F3 0F 10 45 ? 8B D0")).toAbsolute(1, 5).get<decltype(worldToScreen)>();

    createMove = clientModule.FindPattern(SIGNATURE("48 8B C4 48 89 58 10 48 89 48 08 55 56 57 41 54 41 55")).get<std::uintptr_t>();
    levelInit = clientModule.FindPattern(SIGNATURE("48 89 5C 24 ? 57 48 83 EC 20 48 8B 0D ? ? ? ? 48 8B DA 45 33 C9")).get<std::uintptr_t>();
    frameStage = clientModule.FindPattern(SIGNATURE("48 89 5C 24 ? 56 48 83 EC 30 8B 05 ? ? ? ? 8D 5A FF 3B C2 48")).get<std::uintptr_t>();
    mouseInputEnabled = clientModule.FindPattern(SIGNATURE("40 53 48 83 EC 20 80 B9 ? ? ? ? ? 48 8B D9 75 78")).get<std::uintptr_t>();

    getBaseEntity = clientModule.FindPattern(SIGNATURE("8B D3 E8 ? ? ? ? 48 8B F8 48 85 C0 74 76")).toAbsolute(3, 0).get<decltype(getBaseEntity)>();
    getBaseEntityFromHandle = clientModule.FindPattern(SIGNATURE("E8 ? ? ? ? 4D 8B 3E 4C 89 7C 24 ?")).toAbsolute(1, 0).get<decltype(getBaseEntityFromHandle)>();
    getHighestEntityIndex = clientModule.FindPattern(SIGNATURE("33 DB E8 ? ? ? ? 8B 08")).toAbsolute(3, 0).get<decltype(getHighestEntityIndex)>();

    SDL_SetRelativeMouseMode = sdl3.GetProcAddress<decltype(SDL_SetRelativeMouseMode)>("SDL_SetRelativeMouseMode");
    SDL_SetWindowGrab = sdl3.GetProcAddress<decltype(SDL_SetWindowGrab)>("SDL_SetWindowGrab");
    SDL_WarpMouseInWindow = sdl3.GetProcAddress<decltype(SDL_WarpMouseInWindow)>("SDL_WarpMouseInWindow");
}
