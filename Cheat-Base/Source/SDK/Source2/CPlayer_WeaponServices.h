#pragma once

#include "Handle.h"

#include "../Schema.h"

class BasePlayerWeapon;

class CPlayer_WeaponServices {
public:
	SCHEMA(Handle, activeWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon");

	BasePlayerWeapon* getActiveWeapon();
};