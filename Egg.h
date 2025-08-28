#ifndef EGG_H
#define EGG_H
#include "CommonVariable.h"
#include "Entity.h"
#include "Animation.h"

class Egg : public Entity
{
protected:
    sf::Texture nice_egg_;
    sf::Texture broken_egg_;
    sf::Time last_time_eggs_broken_;
    bool is_broken_;
    int speed_;
    float spinning_egg_;
    int v_x_;
    int v_y_;
    bool alive_;

public:
    Egg();
    ~Egg();

    void set_rect_cordinate(const float& x, const float& y);
    void set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height);
    void set_rect_width_and_height(const float& width, const float& height);
    sf::FloatRect get_rect() const override;
    sf::FloatRect get_rect_width_height_with_scale(const double& scale) const;

    void set_v_x(const float& v_x) { v_x_ = v_x; }
    void set_v_y(const float& v_y) { v_y_ = v_y; }
    float get_v_x() const { return v_x_; }
    float get_v_y() const { return v_y_; }
	float get_x() const { return x; }
	float get_y() const { return y; }

    bool get_alive() const { return alive_; }
    void set_alive(const bool& alive) { alive_ = alive; }

    int get_speed() const { return speed_; }
    void set_speed(const int& speed) { speed_ = speed; }

    bool get_is_broken() const { return is_broken_; }
    void set_is_broken(const bool& is_broken) { is_broken_ = is_broken; }

    void move_diagonally();
    void move_horizontally();
    void render(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) override;
    void update() override;
    void free();
};

#endif
//#ifndef EGG_H
//#define EGG_H
//#include "CommonVariable.h"
//#include "Entity.h"
//#include "Animation.h"
//
//class Egg : public Entity
//{
//protected:
//    sf::Texture nice_egg_;
//    sf::Texture broken_egg_;
//    sf::Time last_time_eggs_broken_;
//    bool is_broken_;
//    int speed_;
//    float spinning_angle_;
//    int v_x_;
//    int v_y_;
//    bool alive_;
//
//public:
//    Egg();
//    ~Egg();
//
//    void set_position(float x, float y);
//    void set_size(float width, float height);
//
//    sf::FloatRect get_rect() const override;
//    sf::FloatRect get_rect_with_scale(double scale) const;
//
//    void set_velocity(float vx, float vy) { v_x_ = vx; v_y_ = vy; }
//    float get_vx() const { return v_x_; }
//    float get_vy() const { return v_y_; }
//
//    bool is_alive() const { return alive_; }
//    void set_alive(bool alive) { alive_ = alive; }
//
//    int get_speed() const { return speed_; }
//    void set_speed(int speed) { speed_ = speed; }
//
//    bool is_broken() const { return is_broken_; }
//    void set_broken(bool broken);
//
//    void move_diagonally();
//    void move_horizontally();
//    void render(sf::RenderWindow& window);
//    void draw(sf::RenderWindow& window) override;
//    void update() override;
//    void free();
//};
//
//#endif
