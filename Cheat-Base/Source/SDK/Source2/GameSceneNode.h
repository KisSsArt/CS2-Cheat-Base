#pragma once 

#include "../Netvars.h"

class GameSceneNode {
public:
	NETVAR(Vector, getAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin");
	NETVAR(Vector, origin, "CGameSceneNode", "m_vecOrigin");
};