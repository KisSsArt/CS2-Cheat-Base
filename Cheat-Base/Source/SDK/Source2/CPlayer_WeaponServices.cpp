#include "CPlayer_WeaponServices.h"

#include "BasePlayerWeapon.h"

BasePlayerWeapon* CPlayer_WeaponServices::getActiveWeapon()
{
	return activeWeapon().get<BasePlayerWeapon>();
}
