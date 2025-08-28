#include "LevelManager.h"
#include <cstdlib>
#include <ctime>
#include "Chicken.h"
#include "Asteroid.h"
#include "Boss.h"
#include <random>
#include <iostream>
#include <memory>

LevelManager::LevelManager()
    : currentLevel(1), spawnTimer(0.0f), spawnInterval(1.0f) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void LevelManager::update(float deltaTime) {
    spawnTimer += deltaTime;
    // chỉ log debug
    std::cout << "[DEBUG] Timer=" << spawnTimer
        << " / Interval=" << spawnInterval
        << " | Level=" << currentLevel << std::endl;
}

std::random_device rd;
std::mt19937 rng(rd());
static bool bossSpawned = false;

std::unique_ptr<Entity> LevelManager::spawnEnemy(sf::Vector2u windowSize) {
    if (spawnTimer < spawnInterval) return nullptr; 
    spawnTimer = 0.0f; // Reset timer sau khi spawn

    // Random x trong khoảng an toàn
    std::uniform_int_distribution<int> xDist(50, windowSize.x - 50);

    // Điều chỉnh tốc độ spawn theo level
    if (currentLevel == 1) spawnInterval = 1.0f;
    else if (currentLevel == 2) spawnInterval = 0.7f;
    else if (currentLevel == 3) spawnInterval = 1.5f;

    switch (currentLevel) {
    case 1: { // Chicken
        return std::make_unique<Chicken>(); 
    }
    case 2: { // Asteroid
        return std::make_unique<Asteroid>();
    }
    case 3: { // Boss (spawn 1 lần)
        static bool bossSpawned = false;
        if (!bossSpawned) {
            bossSpawned = true;
            return std::make_unique<Boss>();
        }
        break;
    }
    }

    return nullptr;
}

float LevelManager::getLastSpawnTime() const {
    return spawnTimer;
}

float LevelManager::getSpawnInterval() const {
    return spawnInterval;
}

int LevelManager::getCurrentLevel() const {
    return currentLevel;
}

void LevelManager::setCurrentLevel(int level) {
    currentLevel = level;
    spawnTimer = 0.0f; // reset khi đổi level

    if (currentLevel == 3) bossSpawned = false; // reset boss flag
    std::cout << "LevelManager set to Level " << currentLevel << std::endl;
}
