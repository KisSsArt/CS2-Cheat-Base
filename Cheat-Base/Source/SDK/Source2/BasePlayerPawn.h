#pragma once

#include "BaseEntity.h"

class CPlayer_WeaponServices;

class BasePlayerPawn : public BaseEntity {
public:
	NETVAR(Handle, controller, "C_BasePlayerPawn", "m_hController");
	NETVAR(CPlayer_WeaponServices*, getWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices");
};