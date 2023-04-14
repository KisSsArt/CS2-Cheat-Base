#pragma once

#include <cstdint>

class BaseEntity;

#define ENT_ENTRY_MASK 0x7fff

class Handle {
public:
    BaseEntity* getBaseEntity() const;
    inline int getEntryIndex() const { return index & ENT_ENTRY_MASK; }

    template <typename T>
    inline T* get() const 
    {
        return (T*)(getBaseEntity());
    }

    std::uintptr_t index;
};