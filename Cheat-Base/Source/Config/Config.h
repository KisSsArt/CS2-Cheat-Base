#pragma once

#include <vector>
#include <string>
#include <filesystem>

class Config {
public:
    Config() noexcept;
    void load(size_t) noexcept;
    void load(std::string name) noexcept;
    void save(size_t) const noexcept;
    void add(std::string) noexcept;
    void remove(size_t) noexcept;
    void rename(size_t, std::string) noexcept;
    void reset() noexcept;
    void listConfigs() noexcept;
    void createConfigDir() const noexcept;

    constexpr auto& getConfigs() noexcept
    {
        return configs;
    }

    struct Misc {
        bool bunnyHop{ false };
    } misc;    
    
    struct Visuals {
        bool noScopeCrosshair{ false };
    } visuals;    
    
    struct ESP {
        bool enable{ false };
        bool box{ false };
        bool name{ false };
        bool healthBar{ false };
        bool health{ false };
    } esp;

private:
    std::filesystem::path path;
    std::vector<std::string> configs;
};

inline std::unique_ptr<Config> config;