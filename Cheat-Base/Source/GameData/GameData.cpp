#include "GameData.h"

#include <algorithm>

#include "../Memory/Memory.h"

#include "../SDK/LocalPlayer.h"

#include "../SDK/Source2/GameEntitySystem.h"
#include "../SDK/Source2/CSPlayerPawnBase.h"
#include "../SDK/Source2/CollisionProperty.h"
#include "../SDK/Source2/GameSceneNode.h"

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
            if (playerController->isLocalPlayerController() || !playerController->pawnIsAlive())
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

    auto gameSceneNode = pawn->gameSceneNode();
    if (!gameSceneNode)
        return;

    origin = gameSceneNode->getAbsOrigin();

    obbMins = collision->mins() + origin;
    obbMaxs = collision->maxs() + origin;
}

PlayerData::PlayerData(CSPlayerController* playerController, CSPlayerPawnBase* pawn, unsigned int handle) noexcept : BaseData{ pawn }
{
    this->handle = handle;

    update(playerController, pawn);
}

void PlayerData::update(CSPlayerController* playerController, CSPlayerPawnBase* pawn) noexcept
{
    static_cast<BaseData&>(*this) = { pawn };

    if (playerController->sanitizedPlayerName())
        this->name = playerController->sanitizedPlayerName();

    this->alive = playerController->pawnIsAlive();
    this->health = playerController->pawnHealth();
    this->enemy = pawn->isEnemyTo(localPlayer->team());

    this->team = pawn->team();
}