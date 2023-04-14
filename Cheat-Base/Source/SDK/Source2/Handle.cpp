#include "Handle.h"

#include "GameEntitySystem.h"

class BaseEntity;

BaseEntity* Handle::getBaseEntity() const
{
    return memory->entitySystem->getBaseEntity(getEntryIndex());
}