#include "GameManager.h"
#include "Chicken.h"
#include <cstdlib>

void GameManager::spawnChickenWave(std::list<Entity*>& entities, Animation& sEgg,
    Animation& sChicken1, Animation& sChicken2, Animation& sChicken3,
    Animation& sChicken4, Animation& sChicken5, Animation& sChicken6,Animation& sChicken7)
{
    for (int i = 0; i < 14; i++) {
        Chicken* c = new Chicken(&sEgg, &entities);
        int r = rand() % 7;
        Animation* a[] = { &sChicken1, &sChicken2, &sChicken3, &sChicken4, &sChicken5, &sChicken6, &sChicken7 };
        c->settings(*a[r], 100 + i * 80, 50, -90, 15);
        entities.push_back(c);
    }
}