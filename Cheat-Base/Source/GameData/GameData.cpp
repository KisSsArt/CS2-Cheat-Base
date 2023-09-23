#include "GameData.h"

#include <algorithm>

#include "../Memory/Memory.h"

#include "LocalPlayer.h"

#include "Source2/GameEntitySystem.h"
#include "Source2/CSPlayerPawnBase.h"
#include "Source2/CollisionProperty.h"
#include "Source2/GameSceneNode.h"

static std::vector<PlayerData> playerData;

static auto playerByHandleWritable(int handle) noexcept
{
	const auto it = std::ranges::find(playerData, handle, &PlayerData::handle);
	return it != playerData.end() ? &(*it) : nullptr;
}

void GameData::update() noexcept
{
	Lock lock;

	if (!localPlayer) {
		playerData.clear();
		return;
	}

	const auto highestEntityIndex = memory->entitySystem->getHighestEntityIndex();
	for (int i = 1; i <= highestEntityIndex; ++i) {
		CSPlayerController* playerController = memory->entitySystem->getBaseEntity<CSPlayerController>(i);
		if (!playerController)
			continue;

        if (playerController->isPlayerController())
        {
            if (playerController->isLocalPlayerController())
                continue;

            CSPlayerPawnBase* pawn = playerController->getPawn();
            if (!pawn)
                continue;

            if (const auto player = playerByHandleWritable(pawn->controller().index))
            {
                player->update(playerController, pawn);
            }
            else
            {
                playerData.emplace_back(playerController, pawn, pawn->controller().index);
            }
        }
	}

    std::erase_if(playerData, [](const auto& player) { return memory->entitySystem->getBaseEntityFromHandle(player.handle) == nullptr; });
}

const std::vector<PlayerData>& GameData::getPlayers() noexcept
{
    return playerData;
}

BaseData::BaseData(CSPlayerPawnBase* pawn) noexcept
{
    auto collision = pawn->collision();
    if (!collision)
        return;

    origin = pawn->gameSceneNode()->getAbsOrigin();

    obbMins = collision->mins() + origin;
    obbMaxs = collision->maxs() + origin;

    auto controller = memory->entitySystem->getLocalPlayerController();
    if (!controller)
        return;    

    distanceToLocal = origin.distTo(controller->gameSceneNode()->getAbsOrigin());
}

PlayerData::PlayerData(CSPlayerController* playerController, CSPlayerPawnBase* pawn, unsigned int handle) noexcept : BaseData{ pawn }
{
    this->handle = handle;

    update(playerController, pawn);
}

void PlayerData::update(CSPlayerController* playerController, CSPlayerPawnBase* pawn) noexcept
{
    static_cast<BaseData&>(*this) = { pawn };

    auto playerName = playerController->sanitizedPlayerName();
    if (playerName && strlen(playerName) > 0)
        this->name = playerName;

    this->alive = playerController->pawnIsAlive();
    this->health = playerController->pawnHealth();

    auto localController = memory->entitySystem->getLocalPlayerController();
    if (localController)
    {
        auto localPawn = localController->getPawn();
        if (localPawn)
            this->enemy = pawn->isEnemyTo(localPawn->team());
    }

    this->team = pawn->team();
}