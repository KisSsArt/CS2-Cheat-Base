#pragma once

#include "BasePlayerController.h"

class CSPlayerController : public BasePlayerController {
public:
    SCHEMA(const char*, sanitizedPlayerName, "CCSPlayerController", "m_sSanitizedPlayerName");
    SCHEMA(int, pawnHealth, "CCSPlayerController", "m_iPawnHealth");
    SCHEMA(bool, pawnIsAlive, "CCSPlayerController", "m_bPawnIsAlive");
    SCHEMA(int, pawnArmor, "CCSPlayerController", "m_iPawnArmor");
    SCHEMA(bool, pawnHasHelmet, "CCSPlayerController", "m_bPawnHasHelmet");
};