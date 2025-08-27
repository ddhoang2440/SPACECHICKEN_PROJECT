#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include "Animation.h"

class Entity {
public:
    float x, y, dx, dy, radius_, angle_;
    bool life_;
    std::string name_;
    Animation anim_;
    Entity() : x(0), y(0), dx(0), dy(0), radius_(1), angle_(0), life_(true) {}

    void settings(Animation& a, float X, float Y, float Angle = 0, float radius = 1) {
        anim_ = a;
        x = X;
        y = Y;
        angle_ = Angle;
        radius_ = radius;
    }

    virtual void set_rect_cordinate(float x, float y) { // Thêm hàm ảo
        this->x = x;
        this->y = y;
        anim_.sprite_.setPosition(x, y);
    }

    virtual void update() {}
    virtual void draw(sf::RenderWindow& window) {
        anim_.sprite_.setPosition(x, y);
        anim_.sprite_.setRotation(angle_ + 90);
        window.draw(anim_.sprite_);
    }
    virtual sf::FloatRect get_rect() const {
        return sf::FloatRect(x - radius_, y - radius_, radius_ * 2, radius_ * 2);
    }

    virtual ~Entity() {}
};

#endif