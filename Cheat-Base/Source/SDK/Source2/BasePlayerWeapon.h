#pragma once

#include "../Schema.h"

#include "GameTick.h"
#include "EconEntity.h"

class BasePlayerWeapon : public EconEntity {
public:
	SCHEMA(GameTick_t, nextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick");
	SCHEMA(int, clip1, "C_BasePlayerWeapon", "m_iClip1");

	bool canShoot();
};