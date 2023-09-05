#include "Utils.h"

#include "imgui/imgui.h"

#include "../Interfaces.h"
#include "../Memory/Memory.h"

#include "Source2/EngineClient.h"

#include "Vector.h"

bool worldToScreen(Vector& origin, ImVec2& out)
{
	if (!memory->worldToScreen)
		return false;

	Vector vecOut;
	vecOut.x = out.x;
	vecOut.y = out.y;

	auto result = !(memory->worldToScreen(origin, vecOut));
	auto screenSize = interfaces->engineClient->getScreenSize();
	out.x = ((vecOut.x + 1.0) * 0.5) * (float)screenSize.x;
	out.y = (float)screenSize.y - (((vecOut.y + 1.0) * 0.5) * (float)screenSize.y);
	return result;
}