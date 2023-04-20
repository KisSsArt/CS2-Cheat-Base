#pragma once

struct ImWchar;

#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/imgui_internal.h"

namespace Helpers
{
	ImWchar* getFontGlyphRanges() noexcept;
}