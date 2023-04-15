#pragma once

#include "../Netvars.h"

#include "GameTick.h"
#include "EconEntity.h"

class BasePlayerWeapon : public EconEntity {
public:
	NETVAR(GameTick_t, nextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick");
	NETVAR(int, clip1, "C_BasePlayerWeapon", "m_iClip1");

	bool canShoot();
};