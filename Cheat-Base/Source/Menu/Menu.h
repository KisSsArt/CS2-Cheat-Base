#pragma once

#include <memory>

struct ImFont;

class Menu {
public:
    Menu() noexcept;
    void render() noexcept;

    bool open = true;

    struct {
        ImFont* roboto = nullptr;
    } fonts;
};

inline std::unique_ptr<Menu> menu;