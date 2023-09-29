#include "MinHook.h"

#include <kiero/minhook/include/MinHook.h>

static auto calculateVmtLength(uintptr_t* vmt) noexcept
{
    std::size_t length = 0;
    MEMORY_BASIC_INFORMATION memoryInfo;
    while (VirtualQuery(LPCVOID(vmt[length]), &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
        length++;
    return length;
}

void MinHook::init(void* base) noexcept
{
    this->base = base;
    originals = std::make_unique<uintptr_t[]>(calculateVmtLength(*reinterpret_cast<uintptr_t**>(base)));
}

void MinHook::detour(uintptr_t base, void* fun) noexcept
{
    this->base = (LPVOID)base;
    MH_CreateHook((LPVOID)base, fun, reinterpret_cast<LPVOID*>(&original));
}

void MinHook::hookAt(std::size_t index, void* fun) noexcept
{
    void* orig;
    MH_CreateHook((*reinterpret_cast<void***>(base))[index], fun, &orig);
    originals[index] = uintptr_t(orig);
}

void MinHook::enable(std::size_t index) noexcept
{
    MH_EnableHook((*reinterpret_cast<void***>(base))[index]);
}