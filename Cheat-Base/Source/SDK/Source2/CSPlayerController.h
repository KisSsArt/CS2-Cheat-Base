#pragma once

#include "BasePlayerController.h"

class CSPlayerController : public BasePlayerController {
public:
    NETVAR(const char*, sanitizedPlayerName, "CCSPlayerController", "m_sSanitizedPlayerName");
    NETVAR(int, pawnHealth, "CCSPlayerController", "m_iPawnHealth");
    NETVAR(bool, pawnIsAlive, "CCSPlayerController", "m_bPawnIsAlive");
    NETVAR(int, pawnArmor, "CCSPlayerController", "m_iPawnArmor");
    NETVAR(bool, pawnHasHelmet, "CCSPlayerController", "m_bPawnHasHelmet");
};