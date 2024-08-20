#pragma once 

#include "../Schema.h"

class GameSceneNode {
public:
	SCHEMA(Vector, getAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin");
	SCHEMA(Vector, origin, "CGameSceneNode", "m_vecOrigin");
};