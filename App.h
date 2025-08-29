//#pragma once
//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <vector>
//#include "MainObject.h"
//#include "UIManager.h"
//#include "Menu.h"
//#include "PauseMenu.h"
//#include "SettingMenu.h"
//#include "LevelManager.h"
//#include "CustomCursor.h"
//#include "GameState.h"
//#include "Chicken.h"
//#include "Asteroid.h"
//#include "Boss.h"
//#include "Explosion.h"
//#include "Present.h"
//#include "BackGround.h"
//#define M_PI 3.14159265358979323846
//class App {
//private:
//    // Window and UI
//    sf::RenderWindow window;
//    UIManager ui;
//    Menu menu;
//    PauseMenu pauseMenu;
//    SettingsMenu settingsMenu;
//    CustomCursor cursor;
//    BackGround background;
//
//    // Game objects
//    MainObject player;
//    LevelManager levelManager;
//    sf::Music backgroundMusic;
//    std::vector<Chicken*> chickens;
//    std::vector<Asteroid*> asteroids;
//    std::vector<Boss*> bosses;
//    std::vector<Explosion*> explosions;
//    std::vector<Present*> presents;
//
//    // Game state
//    GameState gameState;
//    int score;
//    int lives;
//    int currentLevel; // Theo dõi level hiện tại (1, 2, 3, 4)
//    bool waitingForEnterEffect;
//    float enterEffectTime;
//    bool isPaused;
//    bool playerWantToPlayAgain;
//    int timesPlayerWantToPlayAgain;
//
//    // Level management
//    int chickenKillCount; // Đếm số gà đã tiêu diệt để kiểm soát chuyển level
//
//    // Private methods
//    void processEvents();
//    void update(float dt);
//    void render();
//    void initGame();
//    void initPlayer();
//    void initAsteroid();
//    void initBoss();
//    void playMusicLevel(int level);
//    bool allLevel1ChickenDead();
//    bool allLevel2AsteroidDead();
//    bool allBossDead();
//    void processChickenVsPlayer(float dt);
//    void init();
//    void processAsteroidVsPlayer(float dt);
//    //void processBossVsPlayer(float dt);
//    void introBeforeLevel(float dt);
//    void cleanUpExplosions();
//    void cleanUpChickens();
//    void add_present(Present* present);
//
//    // Test methods (commented if not used)
//    // bool allEnemiesDead();
//    // void cleanUpObjects();
//    int current_wave_ = 0;
//    void spawn_wave1();
//    void spawn_wave2();
//
//public:
//    App();
//    ~App();
//    void run();
//};
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "MainObject.h"
#include "BackGround.h"
#include "UIManager.h"
#include "Menu.h"
#include "PauseMenu.h"
#include "SettingMenu.h"
#include "CustomCursor.h"
#include "LevelManager.h"
#include "GameState.h"
class App {
public:
    App();
    void run();
    void processEvents();
    void update(float dt);
    void render();
    void initGame();
    void initPlayer();
    void playMusicLevel(int level);
    void introBeforeLevel(float dt);

private:
    sf::RenderWindow window;
    UIManager ui;
    Menu menu;
    CustomCursor cursor;
    PauseMenu pauseMenu;
    SettingsMenu settingsMenu;
    MainObject player;
    LevelManager levelManager;
    BackGround background;
    sf::Music backgroundMusic;
    sf::Font gameOverFont;
    GameState gameState;
    int score;
    int lives;
    int currentLevel;
    bool waitingForEnterEffect;
    float enterEffectTime;
    bool isPaused;
};