#pragma once

#include <memory>
#include <string>

#include "../../Dependencies/imgui/imgui.h"

class Menu {
public:
    Menu() noexcept;
    void render() noexcept;

    bool open = true;
};

inline std::unique_ptr<Menu> menu;