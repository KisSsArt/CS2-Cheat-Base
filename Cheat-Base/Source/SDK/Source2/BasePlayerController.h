#pragma once

#include "BaseEntity.h"

class CSPlayerPawnBase;

class BasePlayerController : public BaseEntity {
public:
	SCHEMA(Handle, pawn, "CBasePlayerController", "m_hPawn");
	SCHEMA(bool, isLocalPlayerController, "CBasePlayerController", "m_bIsLocalPlayerController");

	CSPlayerPawnBase* getPawn();
};