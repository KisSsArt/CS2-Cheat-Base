#pragma once

#include "../Schema.h"

#include "Handle.h"

class EntityIdentity;

class EntityInstance {
public:
	SCHEMA(EntityIdentity*, entity, "CEntityInstance", "m_pEntity");
};