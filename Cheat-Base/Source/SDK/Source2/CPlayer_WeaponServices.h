#pragma once

#include "Handle.h"

#include "../Netvars.h"

class BasePlayerWeapon;

class CPlayer_WeaponServices {
public:
	NETVAR(Handle, activeWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon");

	BasePlayerWeapon* getActiveWeapon();
};