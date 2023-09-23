#pragma once

#include <mutex>
#include <vector>

struct PlayerData;

namespace GameData
{
	void update() noexcept;

    class Lock {
    public:
        Lock() noexcept : lock{ mutex } {};
    private:
        std::scoped_lock<std::mutex> lock;
        static inline std::mutex mutex;
    };

    const std::vector<PlayerData>& getPlayers() noexcept;
}

#include "Vector.h"

class CSPlayerPawnBase;
class CSPlayerController;

struct BaseData {
    BaseData(CSPlayerPawnBase* pawn) noexcept;

    float distanceToLocal = 0.0f;
    Vector origin, obbMins, obbMaxs;
};

struct PlayerData : BaseData {
    PlayerData(CSPlayerController* playerController, CSPlayerPawnBase* pawn, unsigned int handle) noexcept;
    
    void update(CSPlayerController* playerController, CSPlayerPawnBase* pawn) noexcept;

    bool enemy = false;
    bool alive;
    int handle;
    int health;
    int team;
    std::string name;
};