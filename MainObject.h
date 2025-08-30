#pragma once
#include "CommonVariable.h"
#include "Entity.h"
#include "Animation.h"
#include "AmmoObject.h"
#include "Chicken.h"
#include "Boss.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "Present.h"
#include <vector>
#include <memory>
class Chicken;
class AmmoObject;
class Boss;

class MainObject : public Entity
{
protected:
    float x_;
    float y_;
    sf::FloatRect rect_;
    sf::Texture texture_;
    sf::SoundBuffer shoot_sound_buffer_;
    sf::Sound shoot_sound_;
    sf::SoundBuffer eat_wing_sound_buffer_;
    sf::Sound eat_wing_sound_;
    sf::SoundBuffer hit_sound_buffer_;
    sf::Sound hit_sound_;
    sf::SoundBuffer getting_present_sound_buffer_;
    sf::Sound getting_present_sound_;
    std::vector<AmmoObject*> ammo_list;
    std::vector<Explosion*> explosion_list;
    bool is_win_;
    float spinning_angle_;
    int width_of_sprite_;
    int height_of_sprite_;
    int health_;
    int ammo_level_;
    int number_of_wings_;
    bool slow_move_;
    sf::Time slow_move_timer_;
    bool got_hit_;
    int ammo_type_; // 0: normal, 1: advanced, etc.
    float invincible_time_;        // thời gian bất tử còn lại
    const float INVINCIBLE_DURATION = 2.0f; // 1 giây miễn thương
    sf::Clock collisionClock_;
    bool isColliding_;
    float shield_timer_ = 0.f; // thời gian còn lại của khiên
public:
    MainObject();
    ~MainObject();

 
    void load_animation_sprite(const std::string& file);
    void set_clips();
    sf::FloatRect get_rect() const override;
    void render_animation(sf::RenderWindow& window, const double& scale);
    void update() override;
    void handling_movement(sf::Event& event);
    void handling_shooting(sf::Event& event);
    void render_shooting(sf::RenderWindow& window);
    void process_shooting_if_hit_chicken(Chicken* chicken);
    void process_shooting_if_hit_boss(Boss* boss);
    void process_if_eat_wing_rect(Chicken* chicken);
    void process_if_hit_by_chicken(Chicken* chicken);
    void process_if_hit_by_eggs(Chicken* chicken);
    void processing_if_hit_by_boss_egg(Boss* boss);
    void processing_if_hit_by_boss(Boss* boss);
    void process_shooting_if_hit_asteroid(Asteroid* asteroid);
    void process_if_hit_by_asteroid(Asteroid* asteroid);
    bool processing_if_got_present(Present* present);
    void slowly_move_from_bottom();
    void set_slow_move();
    void free();
	float get_x() const { return x; }
	float get_y() const { return y; }
    void set_position(float x, float y);
    void set_rect_cordinate(const float& x, const float& y);
    void set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height);
    void set_rect_width_and_height(const float& width, const float& height);
    sf::FloatRect get_rect_width_height_with_scale(const double& scale) const;
    void start_pull_to_center();
    bool get_is_win() const { return is_win_; }
    void set_is_win(bool win) { is_win_ = win; }
    int get_health() const { return health_; }
    void set_health(int health) { health_ = health; }
    int get_ammo_level() const { return ammo_level_; }
    void set_ammo_level(int level) { ammo_level_ = level; }
    int get_number_of_wings() const { return number_of_wings_; }
    bool get_slow_move() const { return slow_move_; }
    bool get_got_hit() const { return got_hit_; }
    void set_got_hit(bool hit) { got_hit_ = hit; }
    void handle_pull(float dt);
    void set_window(sf::RenderWindow* window_) { window = window_; }

    bool being_pulled_ = false;
    bool pull_to_center_ = false;
    float pull_speed_ = 100.f; // Tốc độ kéo
    sf::Vector2f target_pos_;
    void stop_pull() { being_pulled_ = false; pull_to_center_ = false; }
    bool is_pulling_to_center() const { return pull_to_center_; }
    std::vector<AmmoObject*>& get_ammo_list() { return ammo_list; }
};
