#include "CommonVariable.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
sf::RenderWindow* window = nullptr;
sf::Event event;
const int MIDDLE_X = SCREEN_WIDTH / 2;
const int MIDDLE_Y = SCREEN_HEIGHT / 2;
int times_player_want_to_playagain = 0;
const double MAIN_OBJECT_SCALE = 0.35; // Using the first defined value
const double CHICKEN_OBJECT_SCALE = 1.55; // Using the first defined value
const int number_of_asteroid = 20; // Using the first defined value
const int chicken_number = 30; // Using the first defined value
bool is_paused = false;
bool player_want_to_play_again = false;
int level = 0;
const int boss_number = 2;
const int MAIN_OBJECT_NUMS_FRAME = 4;
const double scale_if_health_is_1 = 0.2;
const double scale_if_health_is_2 = 0.3;
const double scale_if_health_is_3 = 0.5;
const int ASTEROID_WIDTH = 300;
const int ASTEROID_HEIGHT = 300;
const double BOSS_SCALE = 0.5;
const int BOSS_NUMS_FRAME = 10;
const double ATTACK_DISTANCE_SQUARED = 1000000.0;
const sf::Time MOVE_INTERVAL_MS = sf::milliseconds(30);
const int BOSS_NUMBER_OF_EGGS = 9;
const sf::Time BOSS_spritetime = sf::milliseconds(100);
const int EGG_WIDTH = 32;
const int EGG_HEIGHT = 41;
const int EXPLOSION_NUMS_FRAME = 8;
const sf::Time EXPLOSION_spritetime = sf::milliseconds(100);
const double EXPLOSION_SCALE = 2.75;
const int PRESENT_ATOMIC_WIDTH = 44;
const int PRESENT_ATOMIC_HEIGHT = 50;
const int PRESENT_DEFAULT_WIDTH = 49;
const int PRESENT_DEFAULT_HEIGHT = 58;
const float MENU_ENTER_EFFECT_DURATION = 0.4f;
const float PRESENT_SPAWN_INTERVAL = 2.0f;

double lerp(double a, double b, double t) {
    return a + (b - a) * t;
}

bool check_collision_2_rect(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.intersects(b);
}

bool check_collision_circular(const Entity& a, const Entity& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < (a.radius_ + b.radius_);
}