#include "Boss.h"
#include "Egg.h"
#include "Bullet.h"
#include <cmath>

Boss::Boss(Animation* eggReal, std::list<Entity*>* entityListReal, Animation* lazeReal)
{
    name = "boss";
    egg = eggReal;
    entityList = entityListReal;
    laser = lazeReal;
}

void Boss::update()
{
    y += sin(x / 40) * 0.3;

    fireEggTimer += 0.1f;
    fireLaserTimer += 0.1f;

    // Boss đổi kiểu tấn công mỗi vài giây
    if (fireLaserTimer > 200) {
        fireLaserTimer = 0;
        isUsingLaser = !isUsingLaser;
    }

    if (isUsingLaser)
    {
        if ((int)fireLaserTimer % 10 == 0) // bắn mỗi 10 frame
        {
            bullet* b = new bullet();
            b->settings(*laser, x, y + 60, 90, 10);
            b->dy = 4;
            b->dx = 0;
            entityList->push_back(b);
        }
    }
    else
    {
        if (fireEggTimer > 50)
        {
            fireEggTimer = 0;
            Egg* e = new Egg();
            e->settings(*egg, x, y + 60, 90, 15);
            entityList->push_back(e);
        }
    }
}

void Boss::draw(RenderWindow& app)
{
    Entity::draw(app); // Gọi hàm vẽ của lớp cha để vẽ hình ảnh
    RectangleShape hpBack(Vector2f(100, 10));
    hpBack.setFillColor(Color::Red); // Màu nền của thanh máu
    hpBack.setPosition(x - 50, y - 60); // Vị trí của thanh máu

    RectangleShape hpFront(Vector2f(hp, 10));
    hpFront.setFillColor(Color::Green);
    hpFront.setPosition(x - 50, y - 60);

    app.draw(hpBack);
    app.draw(hpFront); // Vẽ thanh máu
}