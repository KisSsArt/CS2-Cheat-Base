#include "InputSystem.h"

#include "../../Interfaces.h"
#include "../../Memory/Memory.h"

void InputSystem::enableInput(bool state)
{
    if (IsRelativeMouseMode())
    {
        memory->SDL_SetRelativeMouseMode(state);
        memory->SDL_SetWindowGrab(GetSDLWindow(), state);
    }
}