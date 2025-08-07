#ifndef PRESENT_H
#define PRESENT_H
#include <SFML/Graphics.hpp>

enum BonusType
{
    ATOMIC_POWER = 0, // Tăng health
    NEUTRON = 1,      // Đạn neutron
    ARROW = 2,        // Đạn arrow
    BORON = 3,        // Đạn boron
    SHIELD =4,
    LIFE =5
};

class Present:public Entity
{
private:
    sf::Texture texture_;
    BonusType name_;
    bool is_on_screen_;
    float speed_;

public:
    Present();
    Present(BonusType name);
    ~Present();

    // Setter và Getter
    void setName(BonusType name);
    BonusType getName() const { return name_; }
    void setShape(sf::Sprite shape) { anim_.sprite_ = shape; }
    sf::Sprite getShape() const { return anim_.sprite_; }
    void setpos(float x, float y) { anim_.sprite_.setPosition(x, y); }
    void set_is_on_screen(bool is_on_screen) { is_on_screen_ = is_on_screen; }
    bool get_is_on_screen() const { return is_on_screen_; }
    void set_speed(float speed) { speed_ = speed; }
    float get_speed() const { return speed_; }

    // Update và render
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    // Get kích thước và vị trí cho va chạm
    sf::FloatRect get_rect() const { return anim_.sprite_.getGlobalBounds(); }
};
#endif