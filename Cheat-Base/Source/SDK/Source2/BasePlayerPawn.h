#pragma once

#include "BaseEntity.h"

class CPlayer_WeaponServices;

class BasePlayerPawn : public BaseEntity {
public:
	SCHEMA(Handle, controller, "C_BasePlayerPawn", "m_hController");
	SCHEMA(CPlayer_WeaponServices*, getWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices");
};