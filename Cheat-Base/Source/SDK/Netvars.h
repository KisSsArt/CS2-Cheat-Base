#pragma once

#include <unordered_map>

#include "fnv.h"

#define NETVAR(type, function, class, prop) \
__forceinline type& function() \
{ \
    const auto offset = Netvar::get(fnv::hash(class), fnv::hash(prop));  \
    return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
} \

#define PNETVAR(type, function, class, prop) \
auto function() \
{ \
    const auto offset = Netvar::get(fnv::hash(class), fnv::hash(prop)); \
    return reinterpret_cast<std::add_pointer_t<type>>((uintptr_t)(this) + offset); \
} \

namespace Netvar
{
    short get(const hash32_t& classHash, const hash32_t& propHash);
    void init();
}
