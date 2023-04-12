#include "BaseEntity.h"

#include "EntityIdentity.h"

#include "../../../Dependencies/fnv.h"

bool BaseEntity::isPlayerController()
{
    EntityIdentity* identity = entity();
    if (!identity)
        return false;

    const char* designerName = identity->designerName();
    if (!designerName)
        return false;

    static constexpr auto cs_player_controller = fnv::hash_32_fnv_const("cs_player_controller");
    return fnv::hash_32_fnv_const(designerName) == cs_player_controller;
}