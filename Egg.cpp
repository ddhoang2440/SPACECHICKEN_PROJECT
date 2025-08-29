#include "Egg.h"
#include <iostream>

Egg::Egg()
    : Entity(), is_broken_(false), speed_(5), spinning_egg_(0), v_x_(0), v_y_(0), alive_(true)
{
    x = 0;
    y = 0;
    radius_ = EGG_WIDTH / 2;
    last_time_eggs_broken_ = sf::Time::Zero;
    if (!nice_egg_.loadFromFile("res/image/egg.png"))
        std::cout << "Failed to load texture: res/image/egg.png" << std::endl;
    if (!broken_egg_.loadFromFile("res/image/egg2.png"))
        std::cout << "Failed to load texture: res/image/egg2.png" << std::endl;
    anim_ = Animation(nice_egg_, 0, 0, EGG_WIDTH, EGG_HEIGHT, 1, 0);
    anim_.sprite_.setPosition(x, y);
}

Egg::~Egg()
{
    free();
}

void Egg::set_rect_cordinate(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
    anim_.sprite_.setPosition(x, y);
}

void Egg::set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height)
{
    this->x = x;
    this->y = y;
    anim_ = Animation(is_broken_ ? broken_egg_ : nice_egg_, 0, 0, is_broken_ ? 76 : 32, is_broken_ ? 57 : 41, 1, 0);
    anim_.sprite_.setPosition(x, y);
    radius_ = (is_broken_ ? 76 : 32) / 2;
}

void Egg::set_rect_width_and_height(const float& width, const float& height)
{
    anim_ = Animation(is_broken_ ? broken_egg_ : nice_egg_, 0, 0, width, height, 1, 0);
    radius_ = width / 2;
}

sf::FloatRect Egg::get_rect() const
{
    float width = is_broken_ ? 76 : EGG_WIDTH;
    float height = is_broken_ ? 57 : EGG_HEIGHT;
    return sf::FloatRect(x - width / 2, y - height / 2, width, height);
}

sf::FloatRect Egg::get_rect_width_height_with_scale(const double& scale) const
{
    float width = is_broken_ ? 76 : EGG_WIDTH;
    float height = is_broken_ ? 57 : EGG_HEIGHT;
    return sf::FloatRect(x - (width * scale) / 2, y - (height * scale) / 2, width * scale, height * scale);
}

void Egg::render(sf::RenderWindow& window)
{
    if (!alive_) return;
    draw(window);
}

void Egg::draw(sf::RenderWindow& window)
{
    if (!alive_) return;
    if (is_broken_)
    {
        sf::Time current_time = sf::Clock().getElapsedTime();
        if (current_time - last_time_eggs_broken_ > sf::milliseconds(2000))
            alive_ = false;
        anim_ = Animation(broken_egg_, 0, 0, 76, 57, 1, 0);
    }
    else
    {
        anim_ = Animation(nice_egg_, 0, 0, EGG_WIDTH, EGG_HEIGHT, 1, 0);
        if (level == 3)
            spinning_egg_ += 30;
        else
            spinning_egg_ += 0;
        if (spinning_egg_ >= 360)
            spinning_egg_ -= 360;
    }
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setRotation(spinning_egg_);
    anim_.update();
    window.draw(anim_.sprite_);
}

void Egg::move_diagonally()
{
    if (!alive_ || is_broken_) return;
    x += v_x_ * speed_;
    y += v_y_ * speed_;
    anim_.sprite_.setPosition(x, y);
    if (y >= 1035)
    {
        is_broken_ = true;
        last_time_eggs_broken_ = sf::Clock().getElapsedTime();
        anim_ = Animation(broken_egg_, 0, 0, 76, 57, 1, 0);
        radius_ = 76 / 2;
    }
}

void Egg::move_horizontally()
{
    if (!alive_ || is_broken_) return;
    x += v_x_ * speed_;
    anim_.sprite_.setPosition(x, y);
    if (x < 0 || x > SCREEN_WIDTH)
        alive_ = false;
}

void Egg::update()
{
    if (!alive_) return;
    if (!is_broken_)
        move_diagonally();
    anim_.update();
}

void Egg::free()
{
    nice_egg_ = sf::Texture();
    broken_egg_ = sf::Texture();
    anim_ = Animation();
}
//#include "Egg.h"
//#include <iostream>
//#include <cmath>
//
//static sf::Clock globalClock; 
//
//Egg::Egg()
//    : Entity(), is_broken_(false), speed_(5),
//    spinning_angle_(0), v_x_(0), v_y_(0), alive_(true)
//{
//    x = 0;
//    y = 0;
//    radius_ = EGG_WIDTH / 2;
//    last_time_eggs_broken_ = sf::Time::Zero;
//
//    if (!nice_egg_.loadFromFile("res/image/egg.png"))
//        std::cout << "Failed to load texture: res/image/egg.png\n";
//    if (!broken_egg_.loadFromFile("res/image/egg2.png"))
//        std::cout << "Failed to load texture: res/image/egg2.png\n";
//
//    anim_ = Animation(nice_egg_, 0, 0, EGG_WIDTH, EGG_HEIGHT, 1, 0);
//    anim_.sprite_.setPosition(x, y);
//}
//
//Egg::~Egg()
//{
//    free();
//}
//
//void Egg::set_position(float x, float y)
//{
//    this->x = x;
//    this->y = y;
//    anim_.sprite_.setPosition(x, y);
//}
//
//void Egg::set_size(float width, float height)
//{
//    anim_ = Animation(is_broken_ ? broken_egg_ : nice_egg_, 0, 0, width, height, 1, 0);
//    radius_ = width / 2;
//    anim_.sprite_.setPosition(x, y);
//}
//
//sf::FloatRect Egg::get_rect() const
//{
//    float width = is_broken_ ? 76.f : EGG_WIDTH;
//    float height = is_broken_ ? 57.f : EGG_HEIGHT;
//    return sf::FloatRect(x - width / 2, y - height / 2, width, height);
//}
//
//sf::FloatRect Egg::get_rect_with_scale(double scale) const
//{
//    float width = (is_broken_ ? 76.f : EGG_WIDTH) * scale;
//    float height = (is_broken_ ? 57.f : EGG_HEIGHT) * scale;
//    return sf::FloatRect(x - width / 2, y - height / 2, width, height);
//}
//
//void Egg::render(sf::RenderWindow& window)
//{
//    if (alive_)
//        draw(window);
//}
//
//void Egg::draw(sf::RenderWindow& window)
//{
//    if (!alive_) return;
//
//    if (is_broken_)
//    {
//        if (globalClock.getElapsedTime() - last_time_eggs_broken_ > sf::milliseconds(2000))
//            alive_ = false;
//
//        anim_ = Animation(broken_egg_, 0, 0, 76, 57, 1, 0);
//    }
//    else
//    {
//        anim_ = Animation(nice_egg_, 0, 0, EGG_WIDTH, EGG_HEIGHT, 1, 0);
//        if (level == 3) spinning_angle_ += 30;
//        if (spinning_angle_ >= 360) spinning_angle_ -= 360;
//    }
//
//    anim_.sprite_.setPosition(x, y);
//    anim_.sprite_.setRotation(spinning_angle_);
//    anim_.update();
//    window.draw(anim_.sprite_);
//}
//
//void Egg::move_diagonally()
//{
//    if (!alive_ || is_broken_) return;
//
//    x += v_x_ * speed_;
//    y += v_y_ * speed_;
//    anim_.sprite_.setPosition(x, y);
//
//    if (y >= 1035) 
//    {
//        is_broken_ = true;
//        last_time_eggs_broken_ = globalClock.getElapsedTime();
//        set_size(76, 57);
//    }
//}
//
//void Egg::move_horizontally()
//{
//    if (!alive_ || is_broken_) return;
//
//    x += v_x_ * speed_;
//    anim_.sprite_.setPosition(x, y);
//
//    if (x < 0 || x > SCREEN_WIDTH)
//        alive_ = false;
//}
//
//void Egg::update()
//{
//    if (!alive_) return;
//    if (!is_broken_)
//        move_diagonally();
//
//    anim_.update();
//}
//
//void Egg::set_broken(bool broken)
//{
//    is_broken_ = broken;
//    if (broken)
//    {
//        last_time_eggs_broken_ = globalClock.getElapsedTime();
//        set_size(76, 57);
//    }
//}
//
//void Egg::free()
//{
//    nice_egg_ = sf::Texture();
//    broken_egg_ = sf::Texture();
//    anim_ = Animation();
//}
