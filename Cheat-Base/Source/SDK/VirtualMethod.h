#pragma once

#include <cstddef>

namespace VirtualMethod
{
    template< typename T >
    __forceinline T getMethod(void* thisptr, uintptr_t idx)
    {
        return reinterpret_cast<T>((*static_cast<uintptr_t**>(thisptr))[idx]);
    }
}