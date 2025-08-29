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

    float targetX_;
    float targetY_;
    bool hasTarget_;
    bool reachedTarget_;
    float formationSpeed_;

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
    void update() override;
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

    void handle_shooting_eggs_toward_player(MainObject* main_object);
    void shooting_eggs_toward_player();
    void update_the_eggs();
    void render_the_eggs(sf::RenderWindow& window);
    void generate_present();
    void moving_back_and_forth();
    void moving_like_a_circle();
    void moving_toward_the_player(MainObject* main_object);
    void moving_toward_the_player();
    void moving_diagonally();
    void moving_parabola();
    void free();

    void setTargetPosition(float x, float y);
    void moveToFormation();
    bool hasReachedTarget() const { return reachedTarget_; }
};

#endif