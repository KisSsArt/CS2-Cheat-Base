#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <Windows.h>

class Client;
class SchemaSystem;
class EngineClient;
class GameResourceService;
class InputSystem;

class Interfaces {
public:
#define GAME_INTERFACE(type, name, module, version) \
type* name = reinterpret_cast<type*>(find(L##module, version));

    GAME_INTERFACE(SchemaSystem, schemaSystem, "schemasystem", "SchemaSystem_001")
    GAME_INTERFACE(EngineClient, engineClient, "engine2", "Source2EngineToClient001")
    GAME_INTERFACE(GameResourceService, gameResourceService, "engine2", "GameResourceServiceClientV001")
    GAME_INTERFACE(InputSystem, inputSystem, "inputsystem", "InputSystemVersion001")

#undef GAME_INTERFACE
private:
    static void* find(const wchar_t* moduleName, const char* name) noexcept
    {
        if (const auto createInterface = reinterpret_cast<std::add_pointer_t<void* __cdecl (const char* name, int* returnCode)>>(GetProcAddress(GetModuleHandleW(moduleName), "CreateInterface")))
            if (void* foundInterface = createInterface(name, nullptr))
                return foundInterface;

        std::exit(EXIT_FAILURE);
    }
};

inline std::unique_ptr<const Interfaces> interfaces;