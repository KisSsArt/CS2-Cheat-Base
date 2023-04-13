#pragma once

#include <cassert>

class BaseEntity;

class LocalPlayer {
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

inline LocalPlayer localPlayer;
