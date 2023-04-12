#pragma once

#include <cstddef>
#include <memory>

class MinHook {
public:
    void init(void* base) noexcept;
    void detour(uintptr_t base, void* fun) noexcept;
    void restore() noexcept {}
    void hookAt(std::size_t index, void* fun) noexcept;
    void enable(std::size_t index) noexcept;

    template<typename T, std::size_t Idx, typename ...Args>
    constexpr auto getOriginal(Args... args) const noexcept
    { 
        return reinterpret_cast<T(__thiscall*)(void*, Args...)>(originals[Idx]);
    }

    template <typename T, typename ...Args>
    auto getOriginal(std::size_t index, Args... args) const noexcept
    {
        return reinterpret_cast<T(__thiscall*)(void*, Args...)>(originals[index]);
    }

    template<typename T, std::size_t Idx, typename ...Args>
    constexpr auto callOriginal(Args... args) const noexcept
    {
        return getOriginal<T, Idx>(args...)(base, args...);
    }

    auto getDetour() noexcept
    {
        return original;
    }

private:
    void* base;
    std::unique_ptr<uintptr_t[]> originals;
    uintptr_t original;
};
