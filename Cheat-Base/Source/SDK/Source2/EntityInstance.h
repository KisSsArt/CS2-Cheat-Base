#pragma once

#include "../Netvars.h"

#include "Handle.h"

class EntityIdentity;

class EntityInstance {
public:
	NETVAR(EntityIdentity*, entity, "CEntityInstance", "m_pEntity");
};