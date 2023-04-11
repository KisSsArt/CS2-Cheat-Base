#include <Windows.h>

#include "../Dependencies/kiero/kiero.h"

#include "Hooks/Hooks.h"

VOID WINAPI OnAttach();

BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD reason, LPVOID reserved)
{
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(moduleHandle);
        CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(OnAttach), NULL, 0, NULL);
        break;

    case DLL_PROCESS_DETACH:
        kiero::shutdown();
        break;
    }
    return TRUE;
}