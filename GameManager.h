#pragma once
#pragma once
#include "Entity.h"
#include "Animation.h"
#include <list>

class GameManager
{
public:
    static void spawnChickenWave(std::list<Entity*>& entities, Animation& sEgg,
        Animation& sChicken1, Animation& sChicken2, Animation& sChicken3,
        Animation& sChicken4, Animation& sChicken5, Animation& sChicken6, Animation& sChicken7);
};