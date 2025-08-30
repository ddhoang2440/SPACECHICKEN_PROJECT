#ifndef APP_H
#define APP_H
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
#endif
