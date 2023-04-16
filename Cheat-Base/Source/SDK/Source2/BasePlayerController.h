#pragma once

#include "BaseEntity.h"

class CSPlayerPawnBase;

class BasePlayerController : public BaseEntity {
public:
	NETVAR(Handle, pawn, "CBasePlayerController", "m_hPawn");
	NETVAR(bool, isLocalPlayerController, "CBasePlayerController", "m_bIsLocalPlayerController");

	CSPlayerPawnBase* getPawn();
};