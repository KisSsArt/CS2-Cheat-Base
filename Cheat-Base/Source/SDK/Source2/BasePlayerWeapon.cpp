#include "BasePlayerWeapon.h"

#include "../../Memory/Memory.h"

#include "GlobalVars.h"

bool BasePlayerWeapon::canShoot()
{
	if (nextPrimaryAttackTick().value > memory->globalVars->tickCount)
		return false;

	if (clip1() <= 0)
		return false;

	return true;
}
