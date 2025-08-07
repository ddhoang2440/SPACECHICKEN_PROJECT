#pragma once
#include "Entity.h"
#include "Constants.h"
#include <list>

class Chicken : public Entity
{
public:
    float firetimer = 0;
    Animation* eggAnim = nullptr;
    std::list<Entity*>* entityList = nullptr;

    float dx = 1.0f;   // Tốc độ bay ngang
    float moveTimer = 0;

    Chicken(Animation* eggOri, std::list<Entity*>* entityListReal);
    void update() override;
};