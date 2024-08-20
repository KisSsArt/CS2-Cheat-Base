#pragma once

#include "../Schema.h"
#include "../Vector.h"

#include "Handle.h"

class CollisionProperty {
public:
	SCHEMA(Vector, mins, "CCollisionProperty", "m_vecMins");
	SCHEMA(Vector, maxs, "CCollisionProperty", "m_vecMaxs");
};