#ifndef ASTEROID_H
#define ASTEROID_H
#include "CommonVariable.h"
#include "Entity.h"
#include "Animation.h"

extern const double scale_if_health_is_1;
extern const double scale_if_health_is_2;
extern const double scale_if_health_is_3;

class Asteroid : public Entity
{
protected:
    int size_level_; // 0=nhỏ,1=vừa,2=lớn
    float radius_;
    sf::Texture texture_;
    sf::Sprite sprite_;
    int random_type;
    int v_x;
    int v_y;
    int speed_;
    int health_;
    bool is_on_screen;
    double spinning_angle_;
	float original_width_;
	float original_height_;
	float width_;
	float height_;
	float frame_width_;
	float frame_height_;
    int nFrames_;

public:
    Asteroid();
    ~Asteroid();
    
    void set_size_level(int lvl);
    void set_rect_cordinate(const float& x, const float& y);
    void set_rect_cordinate_and_width_height(const float& x, const float& y, const float& width, const float& height);
    void set_width_height(const float& width, const float& height);
    sf::FloatRect get_rect() const override;
    void set_speed(const int& speed) { this->speed_ = speed; }
    int get_speed() const { return this->speed_; }
    void set_health(const int& health = 3) { this->health_ = health; }
    int get_health() const { return this->health_; }
    void set_is_on_screen(const bool& is_on_screen) { this->is_on_screen = is_on_screen; }
    bool get_is_on_screen() const { return this->is_on_screen; }
    int get_size_level() const { return size_level_; }
    void draw(sf::RenderWindow& window) override;
    void update() override;
    void moving_diagonal();
    void spinning();
    void free();
    float get_original_height() {
        return original_height_;
    }
    float get_original_width() {
        return original_width_;
    }
};

#endif