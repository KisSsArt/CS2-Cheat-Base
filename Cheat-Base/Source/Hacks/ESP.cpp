#include "ESP.h"

#include <array>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "../Config/Config.h"
#include "../GameData/GameData.h"
#include "../Menu/Menu.h"

#include "LocalPlayer.h"
#include "Utils.h"

static ImDrawList* drawList;

static constexpr auto operator-(float sub, const std::array<float, 3>& a) noexcept
{
    return Vector{ sub - a[0], sub - a[1], sub - a[2] };
}

struct BoundingBox {
private:
    bool valid;
public:
    ImVec2 min, max;
    std::array<ImVec2, 8> vertices;

    BoundingBox(const Vector& mins, const Vector& maxs) noexcept
    {
        min.y = min.x = std::numeric_limits<float>::max();
        max.y = max.x = -std::numeric_limits<float>::max();

        for (int i = 0; i < 8; ++i) {
            Vector point{ i & 1 ? maxs.x : mins.x, i & 2 ? maxs.y : mins.y, i & 4 ? maxs.z : mins.z };
            if (!worldToScreen(point, vertices[i])) {
                valid = false;
                return;
            }

            min.x = std::min(min.x, vertices[i].x);
            min.y = std::min(min.y, vertices[i].y);
            max.x = std::max(max.x, vertices[i].x);
            max.y = std::max(max.y, vertices[i].y);
        }
        valid = true;
    }

    BoundingBox(const BaseData& data) noexcept : BoundingBox{ data.obbMins, data.obbMaxs } {}
    BoundingBox(const Vector& center) noexcept : BoundingBox{ center - 2.0f, center + 2.0f } {}

    operator bool() const noexcept
    {
        return valid;
    }
};

static void drawHealthBar(ImVec2 pos, float height, int health) noexcept
{
    constexpr float width = 1.f;
    
    ImVec2 min = pos;
    ImVec2 max = min + ImVec2{ width, height };

    drawList->PushClipRect(pos + ImVec2{ 0.0f, (100 - health) / 100.0f * height }, pos + ImVec2{ width + 1.0f, height + 1.0f });
    drawList->AddRectFilled(ImFloor(min), ImFloor(max), ImColor(1.0f, 0.0f, 0.0f, 1.0f));
    drawList->PopClipRect();
}

static void renderEnemy(const PlayerData& playerData) noexcept
{
    const BoundingBox bbox{ playerData };
    if (!bbox)
        return;

    if (config->esp.box)
    {
        drawList->AddRect(bbox.min, bbox.max, ImColor(0, 0, 0, 255));
    }
    
    if (config->esp.healthBar)
    {
        drawHealthBar(bbox.min - ImVec2(5.0f, 0.0f), (bbox.max.y - bbox.min.y), playerData.health);
    }

    float text_size = std::clamp(15 * 150 / playerData.distanceToLocal, 10.0f, 15.0f);
    float size_modifier = text_size / 15.0f;

    auto space = 15 * size_modifier;

    auto x_pos = bbox.max.x + 5 * size_modifier;
    auto y_pos = bbox.min.y - 3;

    if (config->esp.health)
    {
        ImVec2 text_size_vec;
        text_size_vec = menu->fonts.roboto->CalcTextSizeA(text_size, FLT_MAX, -1, std::to_string(playerData.health).c_str(), 0, NULL);
        text_size_vec.x = IM_FLOOR(text_size_vec.x + 0.99999f);

        drawList->AddText(menu->fonts.roboto, text_size, ImVec2(x_pos, y_pos), ImColor(0, 0, 0, 255), std::to_string(playerData.health).c_str());
    }

    if (config->esp.name)
    {
        ImVec2 text_size_vec;
        text_size_vec = menu->fonts.roboto->CalcTextSizeA(text_size, FLT_MAX, -1, playerData.name.c_str(), 0, NULL);
        text_size_vec.x = IM_FLOOR(text_size_vec.x + 0.99999f);

        drawList->AddText(menu->fonts.roboto, text_size, ImVec2((bbox.min.x + bbox.max.x) / 2 - text_size_vec.x / 2, bbox.min.y - space), ImColor(0, 0, 0, 255), playerData.name.c_str());
    }
}

void ESP::render(ImDrawList* list)
{
	if (!config->esp.enable)
		return;

	if (!localPlayer)
		return;

	drawList = list;

	for (const auto& player : GameData::getPlayers())
	{
		if (player.alive && player.enemy)
            renderEnemy(player);
	}
}
