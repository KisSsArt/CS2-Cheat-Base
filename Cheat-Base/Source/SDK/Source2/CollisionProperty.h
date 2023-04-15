#pragma once

#include "../Netvars.h"
#include "../Vector.h"

#include "Handle.h"

class CollisionProperty {
public:
	NETVAR(Vector, mins, "CCollisionProperty", "m_vecMins");
	NETVAR(Vector, maxs, "CCollisionProperty", "m_vecMaxs");
};