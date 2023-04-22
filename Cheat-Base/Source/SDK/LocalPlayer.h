#pragma once

#include <cassert>

class BaseEntity;

class LocalEntity {
public:
    void init(BaseEntity** entity) noexcept
    {
        localEntity = entity;
    }

    constexpr operator bool() noexcept
    {
        if (!localEntity)
            return false;
        return *localEntity != nullptr;
    }

    constexpr auto operator->() noexcept
    {
        return *localEntity;
    }

    constexpr auto get() noexcept
    {
        return *localEntity;
    }
private:
    BaseEntity** localEntity = nullptr;
};

inline LocalEntity localPlayer;