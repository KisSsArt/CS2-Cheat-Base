#include "Menu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"

#include "../../Resources/RobotoFont.h"

#include "../Config/Config.h"

#include "Utils.h"
#include "Helpers.h"

ImVec2 menuSize{ 475, 290 };

Menu::Menu() noexcept
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImFontConfig cfg;
    cfg.OversampleV = 3;
    cfg.FontDataOwnedByAtlas = false;

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    fonts.roboto = io.Fonts->AddFontFromMemoryTTF(&Medium, 168644, 15.0f, &cfg, Helpers::getFontGlyphRanges());
    icons_config.GlyphMinAdvanceX = 13.0f;
    icons_config.GlyphMaxAdvanceX = 13.0f;
}

void renderConfigWindow()
{
    static int currentConfig = -1;
    static std::string buffer;

    auto& configItems = config->getConfigs();

    if (static_cast<std::size_t>(currentConfig) >= configItems.size())
        currentConfig = -1;

    ImGui::PushItemWidth(-1);
    if (ImGui::InputTextWithHint("##configname", "config name", &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (currentConfig != -1)
            config->rename(currentConfig, buffer.c_str());
    }

    if (ImGui::ListBox("##listbox", &currentConfig, [](void* data, int idx, const char** out_text) {
        auto& vector = *static_cast<std::vector<std::string>*>(data);
        *out_text = vector[idx].c_str();
        return true;
        }, &configItems, configItems.size(), 5) && currentConfig != -1)
    {
        buffer = configItems[currentConfig];
    }

    if (ImGui::Button("Create config", { -1, 25.0f }) && !buffer.empty())
    {
        config->add(buffer.c_str());
        buffer.clear();
    }

    if (ImGui::Button("Load", { -1, 25.0f }) && currentConfig != -1)
        config->load(currentConfig);

    if (ImGui::Button("Save", { -1, 25.0f }) && currentConfig != -1)
        config->save(currentConfig);

    if (ImGui::Button("Delete", { -1, 25.0f }) && currentConfig != -1)
    {
        config->remove(currentConfig);
        if (static_cast<std::size_t>(currentConfig) < configItems.size())
            buffer = configItems[currentConfig];
        else
            buffer.clear();
    }
    ImGui::PopItemWidth();
}

void Menu::render() noexcept
{
    if (!open)
        return;

    ImGui::SetNextWindowSize(menuSize, ImGuiCond_Once);

    ImGui::Begin("Cheat-Base", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse);
    {
        ImGui::Columns(2, nullptr, false);
        {
            ImGui::Checkbox("Bunny Hop", &config->misc.bunnyHop);
            ImGui::Checkbox("No Scope Crosshair", &config->visuals.noScopeCrosshair);
            ImGui::Spacing();
            ImGui::Checkbox("Enable ESP", &config->esp.enable);
            ImGui::Checkbox("Boxes", &config->esp.box);
            ImGui::Checkbox("Health Bar", &config->esp.healthBar);
            ImGui::Checkbox("Health", &config->esp.health);
            ImGui::Checkbox("Name", &config->esp.name);
        }
        ImGui::NextColumn();
        {
            renderConfigWindow();
        }
        ImGui::Columns(1);
    }
    ImGui::End();
}
