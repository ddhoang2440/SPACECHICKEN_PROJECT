// #ifndef PLAYERS_H
// #define PLAYERS_H
// #include <string>
// #include <SFML/Graphics.hpp>
// #include <SFML/Audio.hpp>
// #include "Entity.h"
// #include "Boss.h"
// #include "Present.h"
// #include "Explosion.h"
// #include "Asteroid.h"
// #include "Bullet.h"
// #include <vector>
// using namespace sf;
// class Players : public Entity{
//     private:
//         sf::Texture texture_;
//         std::vector<Bullet*> ammo_list;
//         sf::SoundBuffer shoot_buffer;
//         sf::Sound shoot_sound;
//         sf::SoundBuffer eat_wing_buffer;
//         sf::Sound eat_wing_sound;
//         sf::SoundBuffer hit_buffer;
//         sf::Sound hit_sound;
//         sf::SoundBuffer present_buffer;
//         sf::Sound getting_present_sound;
        
//         int number_of_wings;
//         bool slow_move;
//         int health;
//         int ammo_type; // 1: BORON, 2: NEUTRON
//         int ammo_level;
//         int shots_;
//         bool immunity_;
//         sf::Clock event_clock_;
//         sf::Clock immunity_timer_;

//         int Score;
//         // sf::SoundBuffer d;
//         // sf::Sound die;
//         // sf::SoundBuffer o;
//         // sf::Sound gameover;
//         float width_of_sprite_;
//         float height_of_sprite_;
//         sf::Clock shoot_timer_;
//         std::vector<Explosion*> explosion_list;
//     public:
//         Players();
//     ~Players();

//     void set_ammo_level(int level) { ammo_level = level; }
//     void set_shots(int shots) { shots_ = shots; }
//     int get_shots() const { return shots_; }
//     int get_health() const { return health; }
//     void set_health(int health) { health = health; }
//     int get_num_of_wings() const { return number_of_wings; }
//     void set_num_of_wings(int num) { number_of_wings = num; }
//     bool get_slow_move() const { return slow_move; }
//     void set_slow_move() { slow_move = true; }

//     void set_clips();
//     void load_animation_sprite(const std::string& file);
//     void render_animation(sf::RenderWindow& window, float scale);
//     void set_rect_cordinate(float x, float y);
//     void set_rect_cordinate_width_and_height(float x, float y, float width, float height);
//     void set_rect_width_and_height(float width, float height);
//     sf::FloatRect get_rect() const;
//     sf::FloatRect get_rect_width_height_with_scale(float scale) const;

//     void handling_movement(sf::Event& event);
//     void handling_shooting(sf::Event& event);
//     void render_shooting();
//     void process_shooting_if_hit_chicken(Chicken* chicken);
//     void process_if_eat_wing_rect(Chicken* chicken);
//     void process_if_hit_by_chicken(Chicken* chicken);
//     void process_if_hit_by_eggs(Chicken* chicken);
//     void process_if_hit_by_asteroid(Asteroid* asteroid);
//     void processing_if_got_present(Present* pickup);
//     void processing_if_hit_by_boss(Boss* boss);
//     void processing_if_hit_by_boss_egg(Boss* boss);
//     void slowly_move_from_bottom();
//     void update();
//     void draw(sf::RenderWindow& window);
//     void free();

//     void set_window(sf::RenderWindow* win) { window_ = win; }

// private:
//     sf::RenderWindow* window_;
// };
// #endif