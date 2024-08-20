#pragma once

#include <unordered_map>

#include "fnv.h"

#define SCHEMA(type, function, class, prop) \
__forceinline type& function() \
{ \
    const auto offset = Schema::get(fnv::hash(class), fnv::hash(prop));  \
    return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
} \

#define PSCHEMA(type, function, class, prop) \
auto function() \
{ \
    const auto offset = Schema::get(fnv::hash(class), fnv::hash(prop)); \
    return reinterpret_cast<std::add_pointer_t<type>>((uintptr_t)(this) + offset); \
} \

namespace Schema
{
    short get(const hash32_t& classHash, const hash32_t& propHash);
    void init();
}
