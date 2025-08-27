#include "Present.h"
#include <iostream>

Present::Present()
    : Entity(), speed_(5), kind_of_present_(static_cast<BonusType>(std::rand() % 6)), is_on_screen_(true)
{
    x = 0;
    y = 0;
    radius_ = PRESENT_DEFAULT_WIDTH / 2;
    load_texture();
    anim_ = Animation(texture_, 0, 0, PRESENT_DEFAULT_WIDTH, PRESENT_DEFAULT_HEIGHT, 1, 0);
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
}

Present::Present(BonusType kind)
    : Entity(), speed_(5), kind_of_present_(kind), is_on_screen_(true)
{
    x = 0;
    y = 0;
    radius_ = PRESENT_DEFAULT_WIDTH / 2;
    load_texture();
    anim_ = Animation(texture_, 0, 0, PRESENT_DEFAULT_WIDTH, PRESENT_DEFAULT_HEIGHT, 1, 0);
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
}

Present::~Present()
{
}

void Present::load_texture()
{
    switch (kind_of_present_)
    {
    case ATOMIC_POWER:
        if (!texture_.loadFromFile("res/image/power_up.png"))
            std::cout << "Failed to load texture: res/image/power_up.png" << std::endl;
        break;
    case NEUTRON:
        if (!texture_.loadFromFile("res/image/neutron_gift.png"))
            std::cout << "Failed to load texture: res/image/neutron_gift.png" << std::endl;
        break;
    case ARROW:
        if (!texture_.loadFromFile("res/image/arrow_gift.png"))
            std::cout << "Failed to load texture: res/image/arrow_gift.png" << std::endl;
        break;
    case BORON:
        if (!texture_.loadFromFile("res/image/boron_gift.png"))
            std::cout << "Failed to load texture: res/image/boron_gift.png" << std::endl;
        break;
    case SHIELD:
        if (!texture_.loadFromFile("res/image/bo.png"))
            std::cout << "Failed to load texture: res/image/shield.png" << std::endl;
        break;
    case LIFE:
        if (!texture_.loadFromFile("res/image/heart.png"))
            std::cout << "Failed to load texture: res/image/heart.png" << std::endl;
        break;
    default:
        std::cout << "Unknown present type: " << kind_of_present_ << std::endl;
        break;
    }
}

void Present::set_rect_cordinate(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
    anim_.sprite_.setPosition(x, y);
}

sf::FloatRect Present::get_rect() const
{
    float width = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_WIDTH : PRESENT_DEFAULT_WIDTH;
    float height = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_HEIGHT : PRESENT_DEFAULT_HEIGHT;
    return sf::FloatRect(x - width / 2, y - height / 2, width, height);
}

void Present::render(sf::RenderWindow& window)
{
    if (!is_on_screen_) return;
    draw(window);
}

void Present::draw(sf::RenderWindow& window)
{
    if (!is_on_screen_) return;
    float width = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_WIDTH : PRESENT_DEFAULT_WIDTH;
    float height = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_HEIGHT : PRESENT_DEFAULT_HEIGHT;
    anim_ = Animation(texture_, 0, 0, width, height, 1, 0);
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setOrigin(width / 2.0f, height / 2.0f);
    anim_.sprite_.setScale(width / texture_.getSize().x, height / texture_.getSize().y);
    window.draw(anim_.sprite_);
}

void Present::update()
{
    if (!is_on_screen_) return;
    y += speed_;
    anim_.sprite_.setPosition(x, y);
    if (y > SCREEN_HEIGHT)
        is_on_screen_ = false;
    anim_.update();
}