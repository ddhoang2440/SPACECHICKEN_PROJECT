#ifndef COMMONVARIABLE_H
#define COMMONVARIABLE_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include "Entity.h"

enum BonusType {
    ATOMIC_POWER,
    NEUTRON,
    ARROW,
    BORON,
    SHIELD,
    LIFE
};

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern sf::RenderWindow* window;
extern sf::Event event;
extern const int MIDDLE_X;
extern const int MIDDLE_Y;
extern int times_player_want_to_playagain;
extern const double MAIN_OBJECT_SCALE;
extern const double CHICKEN_OBJECT_SCALE;
extern const int number_of_asteroid;
extern const int chicken_number;
extern bool is_paused;
extern bool player_want_to_play_again;
extern int level;
extern const int boss_number;
extern const int MAIN_OBJECT_NUMS_FRAME;
extern const int ARROW_WIDTH;
extern const int ARROW_HEIGHT;
extern const double scale_if_health_is_1;
extern const double scale_if_health_is_2;
extern const double scale_if_health_is_3;
extern const int ASTEROID_WIDTH;
extern const int ASTEROID_HEIGHT;
extern const double BOSS_SCALE;
extern const int BOSS_NUMS_FRAME;
extern const double ATTACK_DISTANCE_SQUARED;
extern const sf::Time MOVE_INTERVAL_MS;
extern const int BOSS_NUMBER_OF_EGGS;
extern const sf::Time BOSS_spritetime;
extern const int EGG_WIDTH;
extern const int EGG_HEIGHT;
extern const int EXPLOSION_NUMS_FRAME;
extern const sf::Time EXPLOSION_spritetime;
extern const double EXPLOSION_SCALE;
extern const int PRESENT_ATOMIC_WIDTH;
extern const int PRESENT_ATOMIC_HEIGHT;
extern const int PRESENT_DEFAULT_WIDTH;
extern const int PRESENT_DEFAULT_HEIGHT;
extern const float MENU_ENTER_EFFECT_DURATION;
extern const float PRESENT_SPAWN_INTERVAL;

extern double lerp(double a, double b, double t);

extern bool check_collision_2_rect(const sf::FloatRect& a, const sf::FloatRect& b);
extern bool check_collision_circular(const Entity& a, const Entity& b);

#endif