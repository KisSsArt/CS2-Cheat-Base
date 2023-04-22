#pragma once

#include "EntityInstance.h"
#include "MoveType.h"

enum flags_t : int
{
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_waterjump = (1 << 2),
	fl_ontrain = (1 << 3),
	fl_inrain = (1 << 4),
	fl_frozen = (1 << 5),
	fl_atcontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8),
	fl_inwater = (1 << 9),
	fl_fly = (1 << 10),
	fl_swim = (1 << 11),
	fl_conveyor = (1 << 12),
	fl_npc = (1 << 13),
	fl_godmode = (1 << 14),
	fl_notarget = (1 << 15),
	fl_aimtarget = (1 << 16),
	fl_partialground = (1 << 17),
	fl_staticprop = (1 << 18),
	fl_graphed = (1 << 19),
	fl_grenade = (1 << 20),
	fl_stepmovement = (1 << 21),
	fl_donttouch = (1 << 22),
	fl_basevelocity = (1 << 23),
	fl_worldbrush = (1 << 24),
	fl_object = (1 << 25),
	fl_killme = (1 << 26),
	fl_onfire = (1 << 27),
	fl_dissolving = (1 << 28),
	fl_transragdoll = (1 << 29),
	fl_unblockable_by_player = (1 << 30)
};

class GameSceneNode;
class CollisionProperty;

class BaseEntity : public EntityInstance {
public:
	NETVAR(std::int8_t, team, "C_BaseEntity", "m_iTeamNum");
	NETVAR(int, flags, "C_BaseEntity", "m_fFlags");
	NETVAR(GameSceneNode*, gameSceneNode, "C_BaseEntity", "m_pGameSceneNode");
	NETVAR(CollisionProperty*, collision, "C_BaseEntity", "m_pCollision");
	NETVAR(MoveType, moveType, "C_BaseEntity", "m_MoveType");

	bool isPlayerController();
};