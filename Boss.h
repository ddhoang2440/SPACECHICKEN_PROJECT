#pragma once
#include "Entity.h"
#include "Constants.h"
#include <list>

class Boss : public Entity
{
public:
    Animation* egg = nullptr;
    Animation* laser = nullptr; // Animation cho tia laze
    std::list<Entity*>* entityList = nullptr;
    float fireEggTimer = 0;
    float fireLaserTimer = 0; // Thời gian bắn tia laze
    int hp = 100;
    bool isUsingLaser = false; // Biến để xác định xem boss đang sử dụng tia laze hay trứng

    Boss(Animation* eggReal, std::list<Entity*>* entityListReal, Animation* lazeReal);
    void update() override;
    void draw(RenderWindow& app) override;
};