#include "Boss.h"
#include "Egg.h"
#include "Bullet.h"
#include "Laze.h"
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
    static float timeCounter = 0;
    timeCounter += 0.05f;
    y += sin(timeCounter) * 1.5f;

    fireEggTimer += 0.1f;
    fireLaserTimer += 0.1f;

    // Boss đổi kiểu tấn công mỗi vài giây
    if (fireLaserTimer > 100) {
        fireLaserTimer = 0;
        isUsingLaser = !isUsingLaser;
    }

    if (isUsingLaser)
    {
        if (fireLaserTimer >= 15) {
            fireLaserTimer = 0; // reset
			Laze* laserBullet = new Laze();
            laserBullet->settings(*laser, x, y + 60, 90, 6);
            laserBullet->dy = 8; // Laser nhanh
            laserBullet->dx = 0;
            entityList->push_back(laserBullet);
        }
    }
    else
    {
        if (fireEggTimer >= 50) {
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
    
}
void Boss::drawHp(RenderWindow& app)
{
    RectangleShape backHpBar(Vector2f(200, 20));
    backHpBar.setFillColor(Color::Red);
    backHpBar.setPosition(20, 20);
    app.draw(backHpBar);
    RectangleShape hpBar(Vector2f(hp, 20));
    hpBar.setFillColor(Color::Green);
    hpBar.setPosition(20, 20);
    app.draw(hpBar);
}