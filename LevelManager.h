#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Chicken.h"
#include "Asteroid.h"
#include "Boss.h"
#include "Present.h"
#include "Explosion.h"
#include "MainObject.h"

class LevelManager {
public:
    LevelManager();

    // --------------------- Update & Render -----------------------
    void update(float dt, MainObject& player, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    // --------------------- Spawning -----------------------
    void spawn_wave1();
    void spawn_wave2(MainObject& player); // Asteroid round

    void spawn_round3_wave1(); // Boss round
    void spawn_round3_wave2(MainObject& player); // Boss round
    void spawnAsteroids(int num);
    void updateRound2(float dt, MainObject& player);
    void updateRound3(float dt, MainObject& player);
    // --------------------- Processing -----------------------
    void processChickenVsPlayer(float dt, MainObject& player, sf::RenderWindow& window);
    void processAsteroidVsPlayer(float dt, MainObject& player);
    void processBossVsPlayer(float dt, MainObject& player, sf::RenderWindow& window);

    // --------------------- Utilities -----------------------
    void cleanUpChickens();
    void reset();
    void cleanUpWave();
    // --------------------- Getters -----------------------
    int getScore() const { return score; }
    int getCurrentWave() const { return current_wave_; }
    bool getRound2Active() const { return round2Active; }
    float getAsteroidRoundTime() const { return asteroidRoundTime; }
    void add_present(Present* present) {
        if (present) {
            present->set_is_on_screen(true);
            presents.push_back(std::unique_ptr<Present>(present));
        }
	}
private:
    int current_wave_;
    int score;
    int chickenKillCount;
    bool isPaused;
    // Spawn present 
    sf::RenderWindow window;
    float presentSpawnTimer = 0.f;
    const float presentSpawnInterval = 5.f;
    vector<unique_ptr<Present>> Presents;
    void spawnRandomPresent();
    // Round 1: chickens
    std::vector<std::unique_ptr<Chicken>> chickens;

    // Round 2: asteroids
    std::vector<std::unique_ptr<Asteroid>> asteroids;
    bool round2Active;
	  bool round3Active;
    float asteroidRoundTime;     // Duration of round 2 in seconds
    float asteroidRoundElapsed;  // Time elapsed since round 2 started

    // Round 3: boss
    std::vector<std::unique_ptr<Boss>> bosses;

    // Shared: presents & explosions
    std::vector<std::unique_ptr<Present>> presents;
    std::vector<std::unique_ptr<Explosion>> explosions;
};

#endif
