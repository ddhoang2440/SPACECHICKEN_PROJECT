#include "Chicken.h"
#include "Egg.h"
#include <cmath>

Chicken::Chicken(Animation* eggOri, std::list<Entity*>* entityListReal)
{
    name = "chicken";
    eggAnim = eggOri;
    entityList = entityListReal;
}

void Chicken::update()
{
    moveTimer += 0.1f;
    x += dx; // di chuyển ngang
    y += sin(x / 20) * 0.5f; // hiệu ứng nhấp nhô nhẹ

    // Nếu tới biên, đổi hướng và xuống hàng
    if (x < 50 || x > W - 50) {
        dx = -dx;
        y += 20; // đi xuống 1 bước
    }

    // Bắn trứng theo thời gian
    firetimer += 0.1f;
    if (firetimer > 60) {
        firetimer = 0;
        Egg* e = new Egg();
        e->settings(*eggAnim, x, y + 30, 90, 10);
        entityList->push_back(e);
    }
}