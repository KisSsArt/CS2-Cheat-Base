#pragma once

#include "../Vector.h"
#include "../Pad.h"

#include "UserCmd.h"

class Input {
public:
	PAD(16824) //0x0000
	bool unknown1; //0x41B8
	PAD(4) //0x41B9
	bool inThirdperson; //0x41BD
	PAD(2) //0x41BE
	float unknown2; //0x41C0
	Vector thirdpersonAngles; //0x41C4

	UserCmd* getUserCmd(void* a1, std::uint32_t a2)
	{
		std::int64_t v5 = 0x460 * a2;
		std::int64_t v6 = *reinterpret_cast<std::int64_t*>(reinterpret_cast<std::uintptr_t>(a1) + v5 + 0x41E0);
		std::int64_t v7 = 0x70 * (0x9A * a2 + v6 % 0x96) + reinterpret_cast<std::uintptr_t>(a1);
		std::uint64_t split_screen = v7 + 0x10;
		return reinterpret_cast<UserCmd*>(split_screen);
	}
};