#ifndef CHICKEN_H
#define CHICKEN_H
#include "CommonVariable.h"
#include "Entity.h"
#include "Animation.h"
#include "Egg.h"
#include "Present.h"
#include "MainObject.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
class MainObject;
enum class MovementPattern { NONE, UP_DOWN, CIRCLE, DIAGONAL, PARABOLA };
enum class ShootingMode { NONE, DOWNWARD, TOWARD_PLAYER };
class Chicken : public Entity
{
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

    std::vector<Egg*> eggs_list_;
    Present* present_;
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
	int y_direction_;
    double angle_;
    double angle_increment_;
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
    float shoot_timer_ = 0.0f;
    float shoot_interval_ = 2.0f;
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

    void render_animation(sf::RenderWindow& window, const double& scale);
    void update(float dt);
    void draw(sf::RenderWindow& window) override;

    bool get_has_wing() const { return has_wing_; }
    void set_speed(const int& speed) { speed_ = speed; }
    int get_speed() const { return speed_; }
    std::vector<Egg*>& get_eggs_list() { return eggs_list_; }
    bool get_is_on_screen() const { return is_on_screen_; }
    void set_is_on_screen(bool is_on_screen) { is_on_screen_ = is_on_screen; }
    bool get_alive() const { return health_ > 0 && is_on_screen_; }
    void set_alive(bool alive) { is_on_screen_ = alive; }
    void play_hit_sound();
    void play_laying_eggs_sound();
    void set_health(const int& health) { health_ = health; }
    int get_health() const { return health_; }
    bool get_has_a_present() const { return has_present_; }
    void set_has_present(bool has_present) { has_present_ = has_present; }
    Present* get_present() const { return has_present_ ? present_ : nullptr; }
    sf::FloatRect get_wing_rect() const { return wing_rect_; }
    sf::Time get_last_egg_time() const { return last_egg_time_; }

    void handle_shooting_eggs_downward(float dt);
    void handle_shooting_eggs_toward_player(MainObject* main_object,float dt);
    void shooting_eggs_toward_player();
    void update_the_eggs();
    void render_the_eggs(sf::RenderWindow& window);
    void generate_present();
    void moving_back_and_forth();
	void moving_up_and_down();
    void moving_like_a_circle();
    void moving_toward_the_player(MainObject* main_object);
    void moving_toward_the_player();
    void moving_diagonally();
    void moving_parabola();
    void free();
	void update_eggs(float dt);

    void set_y_bounds(float min_y, float max_y ) {
        
        min_y_ = min_y;
        max_y_ = max_y;
     /*   center_x_ = center_x;
        center_y_ = center_y;*/
    }
    void set_circle_properties(float center_x, float center_y) {
        center_x_ = center_x;
        center_y_ = center_y;
	}
    void set_radius(float radius) { radius_ = radius; }
	void set_y_direction(int y) { y_direction_ = y; }
    double get_angle() const { return angle_; }
    void set_angle(double angle) { angle_ = angle; }
    double get_angle_increment_() const { return angle_increment_; }
    void set_angle_increment_(double angle) { angle_increment_ = angle; }

    void set_movement_pattern(MovementPattern pattern) { movement_pattern_ = pattern; }
    MovementPattern get_movement_pattern() const { return movement_pattern_; }

    void set_shooting_mode(ShootingMode mode) { shooting_mode_ = mode; }
    ShootingMode get_shooting_mode() const { return shooting_mode_; }
};


#endif
//#ifndef CHICKEN_H
//#define CHICKEN_H
//
//#include "CommonVariable.h"
//#include "Entity.h"
//#include "Animation.h"
//#include "Egg.h"
//#include "Present.h"
//#include "MainObject.h"
//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <vector>
//#include <memory>
//
//class MainObject;
//
//enum class MovementPattern { NONE, UP_DOWN, CIRCLE, DIAGONAL, PARABOLA, BACK_FORTH, TOWARD_PLAYER };
//enum class ShootingMode { NONE, DOWNWARD, TOWARD_PLAYER };
//
//class Chicken : public Entity
//{
//protected:
//    static const int CHICKEN_NUMS_FRAME = 18;
//    static const sf::Time CHICKEN_spritetime;
//    static int CHICKEN_spriteIndex;
//
//    sf::Texture texture_;
//    sf::Texture wing_texture_;
//    sf::SoundBuffer chicken_got_hit_buffer_;
//    sf::Sound chicken_got_hit_sound_;
//    sf::SoundBuffer chicken_laying_eggs_buffer_;
//    sf::Sound chicken_laying_eggs_sound_;
//    sf::SoundBuffer eggs_get_destroyed_buffer_;
//    sf::Sound eggs_get_destroyed_sound_;
//
//    std::vector<std::unique_ptr<Egg>> eggs_list_;
//    std::unique_ptr<Present> present_;
//
//    bool is_on_screen_;
//    bool has_present_;
//    bool has_wing_;
//    int health_;
//    int max_health_;
//    int egg_shooting_prob_;
//    int speed_;
//    int wing_fall_speed_;
//    int width_of_sprite_;
//    int height_of_sprite_;
//    float v_x_;
//    float v_y_;
//    int x_direction_;
//    int y_direction_;
//    double angle_;
//    double angle_increment_;
//    double scale_;
//    int damage_;
//
//    MovementPattern movement_pattern_;
//    ShootingMode shooting_mode_;
//
//    float min_y_;
//    float max_y_;
//    float circle_radius_;
//    float circle_center_x_;
//    float circle_center_y_;
//
//    sf::Time last_move_time_;
//    sf::Time last_egg_time_;
//    sf::Time broken_egg_time_;
//    sf::Time last_wing_time_;
//    sf::FloatRect wing_rect_;
//
//public:
//    Chicken();
//    virtual ~Chicken();
//
//    void set_clips();
//    void load_animation_sprite(const std::string& file);
//    void set_position(float x, float y);
//    void set_size(float width, float height);
//    sf::FloatRect get_rect() const override;
//    sf::FloatRect get_scaled_rect(double scale) const;
//
//    void render(sf::RenderWindow& window) override;
//    void update(float dt) override;
//
//    // Getters and setters
//    bool has_wing() const { return has_wing_; }
//    void set_speed(int speed) { speed_ = speed; }
//    int get_speed() const { return speed_; }
//    const std::vector<std::unique_ptr<Egg>>& get_eggs() const { return eggs_list_; }
//    bool is_on_screen() const { return is_on_screen_; }
//    void set_on_screen(bool on_screen) { is_on_screen_ = on_screen; }
//    bool is_alive() const { return health_ > 0 && is_on_screen_; }
//    int get_health() const { return health_; }
//    void set_health(int health) { health_ = std::min(health, max_health_); }
//    bool has_present() const { return has_present_; }
//    void set_has_present(bool has_present) { has_present_ = has_present; }
//    Present* get_present() { return has_present_ ? present_.get() : nullptr; }
//
//    void set_movement_pattern(MovementPattern pattern) { movement_pattern_ = pattern; }
//    MovementPattern get_movement_pattern() const { return movement_pattern_; }
//
//    void set_shooting_mode(ShootingMode mode) { shooting_mode_ = mode; }
//    ShootingMode get_shooting_mode() const { return shooting_mode_; }
//
//    void set_y_bounds(float min_y, float max_y) { min_y_ = min_y; max_y_ = max_y; }
//    void set_circle_properties(float center_x, float center_y, float radius, float angle_speed = 1.0f);
//
//    void play_hit_sound();
//    void play_laying_eggs_sound();
//
//    void handle_shooting(float dt, MainObject* player = nullptr);
//    void update_eggs(float dt);
//    void render_eggs(sf::RenderWindow& window);
//    void generate_present();
//
//    // Movement patterns
//    void move_up_down(float dt);
//    void move_circle(float dt);
//    void move_diagonal(float dt);
//    void move_parabola(float dt);
//    void move_back_forth(float dt);
//    void move_toward_player(float dt, MainObject* player);
//
//    void take_damage(int damage);
//    void destroy();
//
//private:
//    void cleanup();
//};
//
//#endif