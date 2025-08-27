#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Entity.h"

class LevelManager {
public:
    LevelManager();
    void update(float deltaTime);
    std::unique_ptr<Entity> spawnEnemy(sf::Vector2u windowSize);
    float getLastSpawnTime() const;
    float getSpawnInterval() const; // Thêm getter
    int getCurrentLevel() const;
    void setCurrentLevel(int level);
private:
    int currentLevel;
    float spawnTimer;
    float spawnInterval;
    float levelTime = 0.0f;
};

#endif