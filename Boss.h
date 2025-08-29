#ifndef BOSS_H
#define BOSS_H
#include "CommonVariable.h"
#include "Entity.h"
#include "Animation.h"
#include "Egg.h"
#include "Present.h"
#include "MainObject.h"
#include <vector>
#include <algorithm>
class MainObject;
class Boss : public Entity
{
protected:
    static const sf::Time BOSS_spritetime;
    static int BOSS_spriteIndex;

    sf::Texture texture_;
    sf::SoundBuffer shooting_egg_sound_buffer_;
    sf::Sound shooting_egg_sound_;
    sf::SoundBuffer boss_hit_buffer_;
    sf::Sound boss_hit_;
    std::vector<Egg*> egg_list_;
    Present* present_;
    sf::FloatRect health_bar_rect_;
    sf::Color bgColor_;
    sf::Color fgColor_;
    int health_bar_width_;
    int health_bar_height_;
    int health_;
    int speed_;
    int width_of_sprite_;
    int height_of_sprite_;
    float v_x_;
    float v_y_;
    int dir_x_;
    int dir_y_;
    int x_direction_;
    bool is_on_screen_;
    bool has_present_;
    sf::Time last_egg_firing_time_;
    sf::Time last_move_time_;
    std::string prev_collision_dir_;

    float targetX_;
    float targetY_;
    bool hasTarget_;
    bool reachedTarget_;
    float entranceSpeed_;
    bool isEntering_;

public:
    Boss();
    ~Boss();

    void set_clips();
    void load_animation_sprite(const std::string& file);
    void set_rect_cordinate(const float& x, const float& y);
    void set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height);
    void set_rect_width_and_height(const float& width, const float& height);
    sf::FloatRect get_rect() const override;
    sf::FloatRect get_rect_width_height_with_scale(const double& scale) const;
    void draw(sf::RenderWindow& window) override;
    void update() override;

    std::vector<Egg*>& get_egg_list() { return egg_list_; }
    Present* get_present() const { return has_present_ ? present_ : nullptr; }
    const sf::Texture& get_texture() const { return texture_; }
    bool get_has_a_present() const { return has_present_; }
    void set_has_present(bool has_present) { has_present_ = has_present; }
    bool get_is_on_screen() const { return is_on_screen_; }
    void set_is_on_screen(bool is_on_screen) { is_on_screen_ = is_on_screen; }
    int get_health() const { return health_; }
    void set_health(int health) { health_ = health; }

    void render_health_bar(sf::RenderWindow& window);
    void moving_toward_player(MainObject* main_object);
    void firing_eggs();
    void update_the_eggs();
    void render_the_eggs(sf::RenderWindow& window);
    void play_hit_sound();
    void play_laying_eggs_sound();
    void generate_present();

    void setTargetPosition(float x, float y);
    void performEntrance();
    bool hasFinishedEntrance() const { return reachedTarget_; }
    void startBattleMode() { isEntering_ = false; }
};

#endif