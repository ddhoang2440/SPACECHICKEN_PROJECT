#ifndef CHICKEN_H
#define CHICKEN_H
#include "CommonVariable.h"
#include "Entity.h"
#include "Animation.h"
#include "Egg.h"
#include "Present.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <random>
#include <filesystem>
#include "MainObject.h"

#define M_PI 3.14159265358979323846

enum class MovementPattern {
    UP_DOWN,
    CIRCLE,
    DIAGONAL,
    PARABOLA,
    DOWNWARD,
    TOWARD_PLAYER,
	BACK_AND_FORTH
};

enum class ShootingMode {
    NONE,
    DOWNWARD,
    TOWARD_PLAYER
};

class MainObject; // Forward declaration

class Chicken : public Entity {
protected:
    static const int CHICKEN_NUMS_FRAME = 18;
    static const sf::Time CHICKEN_spritetime;
    static int CHICKEN_spriteIndex;

    sf::Texture texture_;
    sf::Texture wing_texture_;
    sf::SoundBuffer chicken_got_hit_buffer_;
    sf::Sound chicken_got_hit_sound_;
    sf::SoundBuffer chicken_laying_eggs_buffer_;
    sf::Sound chicken_laying_eggs_sound_;
    sf::SoundBuffer eggs_get_destroyed_buffer_;
    sf::Sound eggs_get_destroyed_sound_;
    std::vector<std::unique_ptr<Egg>> eggs_list_;
    unique_ptr<Present> present_;
    bool is_on_screen_;
    bool has_present_;
    bool has_wing_;
    int health_;
    int egg_shooting_prob_;
    int speed_;
    int wing_fall_speed_;
    int width_of_sprite_;
    int height_of_sprite_;
    float v_x_;
    float v_y_;
    int dir_x_;
    int dir_y_;
    int x_direction_;
    float y_direction_;
    double angle_;
    double angle_increment_;
    float radius_;
    float angle_speed_;
    double scale_;
    double direction_;
    int damage_;
    int type_;
    sf::Time last_move_time_;
    sf::Time last_egg_time_;
    sf::Time broken_egg_time_;
    sf::Time last_wing_time_;
    sf::FloatRect wing_rect_;
    MovementPattern movement_pattern_;
    ShootingMode shooting_mode_;
    float min_y_;
    float max_y_;
    float center_x_;
    float center_y_;
    float shoot_timer_;
    float shoot_interval_;
    std::mt19937 rng_; // Random number generator
    std::uniform_int_distribution<int> dist_100_; // 0-100
    std::uniform_int_distribution<int> dist_6_; // 0-5
public:
    Chicken();
    ~Chicken();

    void set_clips();
    void load_animation_sprite(const std::string& file);
    void set_rect_cordinate(const float& x, const float& y);
    void set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height);
    void set_rect_width_and_height(const float& width, const float& height);
    sf::FloatRect get_rect() const override;
    sf::FloatRect get_rect_width_height_with_scale(const double& scale) const;

    void set_health(int h) { health_ = h; }
    int get_health() const { return health_; }
    void set_is_on_screen(bool on) { is_on_screen_ = on; }
    bool get_is_on_screen() const { return is_on_screen_; }
    void set_movement_pattern(MovementPattern pattern) { movement_pattern_ = pattern; }
    void set_shooting_mode(ShootingMode mode) { shooting_mode_ = mode; }
    void set_angle(float angle) { angle_ = angle; }
    void set_center(float cx, float cy) { center_x_ = cx; center_y_ = cy; }
    void set_min_max_y(float min, float max) { min_y_ = min; max_y_ = max; }
    void set_circle_parameters(float center_x, float center_y, float radius, float speed) {
        center_x_ = center_x;
        center_y_ = center_y;
        radius_ = radius;
        angle_speed_ = speed;
	}
	  void set_angle_increment_(float inc) { angle_increment_ = inc; }
    void set_speed(const int& speed) { speed_ = speed; }
    void set_y_direction(int y) { y_direction_ = y; }
    void set_present(std::unique_ptr<Present> p) { present_ = std::move(p); has_present_ = !!p; }
	  void set_has_present(bool has) { has_present_ = has; if (!has) present_.reset(); }
    Present* get_present() const { return has_present_ ? present_.get() : nullptr; }
	  ShootingMode get_shooting_mode() const { return shooting_mode_; }
    bool get_has_a_present() const { return has_present_; }
    sf::FloatRect get_wing_rect() const { return wing_rect_; }
    sf::Time get_last_egg_time() const { return last_egg_time_; }
    bool get_has_a_wing() const { return has_wing_; }
    bool get_alive() const { return health_ > 0 && is_on_screen_; }
    void set_alive(bool alive) {
        is_on_screen_ = alive;
    }
    void set_y_bounds(float min_y, float max_y) {

        min_y_ = min_y;
        max_y_ = max_y;
        /*   center_x_ = center_x;
           center_y_ = center_y;*/

    }
    void set_circle_properties(float center_x, float center_y) {
        center_x_ = center_x;
        center_y_ = center_y;
    }
	void set_radius(float r) { radius_ = r; }
    int get_damage() const { return damage_; }
	sf::FloatRect get_wing_rect() const { return wing_rect_; }
	MovementPattern get_movement_pattern() const { return movement_pattern_; }
    std::vector<std::unique_ptr<Egg>>& get_egg_list() { return eggs_list_; }

    void set_clips();
    void load_animation_sprite(const std::string& file);
    void render_animation(sf::RenderWindow& window, const double& scale = CHICKEN_OBJECT_SCALE);
    void draw(sf::RenderWindow& window) override;
    void update(float dt);
    void update_eggs(float dt);
    void render_the_eggs(sf::RenderWindow& window);
    void handle_shooting_eggs_downward(float dt);
    void handle_shooting_eggs_toward_player(MainObject* main_object, float dt);
    void update_the_eggs(float dt);
    void play_hit_sound();
    void play_laying_eggs_sound();
    double get_angle() const { return angle_; }
    std::unique_ptr<Present> generate_present();
    void moving_diagonally();
    void moving_parabola();
    void moving_up_and_down();
    void moving_back_and_forth();
    void moving_like_a_circle();
    void moving_toward_the_player(MainObject* main_object);
    void moving_down();
    void free();
};

#endif
