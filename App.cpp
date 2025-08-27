#include "App.h"
#include <iostream>
#include <memory>
#include "CommonVariable.h"
#include "Chicken.h"
#include "Asteroid.h"
#include "Boss.h"

App::App()
    : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Space Chicken"),
    ui(),
    menu(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)),
    pauseMenu(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)),
    settingsMenu(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)),
    player(),
    levelManager(),
    cursor(),
    background(),
    gameState(GameState::MENU),
    score(0),
    lives(3),
    currentLevel(1), // Bắt đầu từ level 1
    waitingForEnterEffect(false),
    enterEffectTime(0.f),
    isPaused(false),
    playerWantToPlayAgain(false),
    timesPlayerWantToPlayAgain(0),
    chickenKillCount(0)
{
    window.setVerticalSyncEnabled(true);
    if (!sf::Shader::isAvailable()) {
        std::cerr << "Shaders are not available" << std::endl;
        window.close();
    }
    window.setMouseCursorVisible(false);
    cursor.loadFromFile("assets/cursor/crosshair.png");
    initPlayer();
    background.loading_background("assets/images/background.png");
    playMusicLevel(currentLevel);
    window.setFramerateLimit(60); // Giới hạn FPS để ổn định
}

void App::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void App::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        switch (gameState) {
        case GameState::MENU:
            menu.handleEvent(event, window);
            if (!waitingForEnterEffect) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    menu.activate();
                    waitingForEnterEffect = true;
                    enterEffectTime = 0.f;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    int clicked = menu.handleMouseClick(mousePos);
                    if (clicked == 0) { // New Game
                        menu.activate();
                        waitingForEnterEffect = true;
                        enterEffectTime = 0.f;
                    }
                    else if (clicked == 2) { // Settings
                        gameState = GameState::SETTINGS;
                        settingsMenu.show();
                    }
                    else if (clicked == 3) { // Quit
                        window.close();
                    }
                }
            }
            break;

        case GameState::PLAYING:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    isPaused = !isPaused;
                    gameState = isPaused ? GameState::PAUSED : GameState::PLAYING;
                    if (isPaused) pauseMenu.show();
                    else pauseMenu.hide();
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (player.get_health() <= 0 && event.key.code == sf::Keyboard::C) {
                    playerWantToPlayAgain = true;
                    timesPlayerWantToPlayAgain++;
                }
            }

            if (!isPaused && player.get_health() > 0) {
                player.handling_movement(event);
                player.handling_shooting(event);
            }
            break;

        case GameState::PAUSED:
            pauseMenu.handleEvent(event, window);
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                int clicked = pauseMenu.handleMouseClick(mousePos);
                if (clicked == 0) { // Resume
                    gameState = GameState::PLAYING;
                    isPaused = false;
                    pauseMenu.hide();
                }
                else if (clicked == 1) { // Main Menu
                    gameState = GameState::MENU;
                    isPaused = false;
                    pauseMenu.hide();
                    currentLevel = 1;
                    playMusicLevel(currentLevel);
                }
                else if (clicked == 2) { // Quit
                    window.close();
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                gameState = GameState::MENU;
                isPaused = false;
                pauseMenu.hide();
                currentLevel = 1;
                playMusicLevel(currentLevel);
            }
            break;

        case GameState::SETTINGS:
            settingsMenu.handleEvent(event, window);
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                int clicked = settingsMenu.handleMouseClick(mousePos);
                if (clicked == 3) { // Back to Main Menu
                    gameState = GameState::MENU;
                    settingsMenu.hide();
                    currentLevel = 1;
                    playMusicLevel(currentLevel);
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                gameState = GameState::MENU;
                settingsMenu.hide();
                currentLevel = 1;
                playMusicLevel(currentLevel);
            }
            break;
        }
    }
}

void App::update(float dt) {
    cursor.update(window);
    settingsMenu.update(dt);
    backgroundMusic.setVolume(settingsMenu.getMasterVolume());
    player.set_got_hit(settingsMenu.getSoundEffectsEnabled());

    for (auto* chicken : chickens) chicken->set_alive(settingsMenu.getSoundEffectsEnabled());
    for (auto* boss : bosses) boss->set_is_on_screen(settingsMenu.getSoundEffectsEnabled());

    switch (gameState) {
    case GameState::MENU:
        menu.update(dt);
        if (waitingForEnterEffect) {
            enterEffectTime += dt;
            if (enterEffectTime >= MENU_ENTER_EFFECT_DURATION) {
                waitingForEnterEffect = false;
                if (menu.getSelectedIndex() == 0) { // New Game
                    gameState = GameState::PLAYING;
                    initGame();
                    currentLevel = 1;
                    levelManager.setCurrentLevel(currentLevel); // Đồng bộ level
                    playMusicLevel(currentLevel);
                }
                else if (menu.getSelectedIndex() == 3) { // Settings
                    gameState = GameState::SETTINGS;
                    settingsMenu.show();
                }
                else if (menu.getSelectedIndex() == 1) { // Quit
                    window.close();
                }
            }
        }
        break;

    case GameState::PLAYING: {
        cleanUpExplosions();
        cleanUpChickens();
        background.update_background_scroll();
        player.update();

        for (auto* exp : explosions) {
            if (exp->get_is_on_screen()) exp->update();
        }

        // --- Spawn theo LevelManager ---
        levelManager.update(dt);

        auto newEnemy = levelManager.spawnEnemy(window.getSize());
        if (newEnemy) {
            if (auto* chicken = dynamic_cast<Chicken*>(newEnemy.get())) {
                chicken->load_animation_sprite("res/image/chicken123.png");
                chicken->set_clips();
                chicken->set_rect_cordinate(rand() % SCREEN_WIDTH, -100);
                chicken->set_rect_width_and_height(75, 68);
                chicken->set_is_on_screen(true);
                chicken->set_health(1);
                chicken->set_speed(3);
                chicken->generate_present();
                chickens.push_back(chicken);
                newEnemy.release(); // nhả ownership, để vector quản lý
            }
            else if (auto* asteroid = dynamic_cast<Asteroid*>(newEnemy.get())) {
                asteroid->set_rect_cordinate(rand() % SCREEN_WIDTH, -100 - (rand() % 500));
                asteroid->set_is_on_screen(true);
                asteroid->set_speed(2);
                asteroid->set_health(3);
                asteroids.push_back(asteroid);
                newEnemy.release();
            }
            else if (auto* boss = dynamic_cast<Boss*>(newEnemy.get())) {
                boss->load_animation_sprite("res/image/boss.png");
                boss->set_clips();
                boss->set_rect_cordinate(rand() % (SCREEN_WIDTH - 200) + 100, 100);
                boss->set_is_on_screen(true);
                boss->set_health(100);
                bosses.push_back(boss);
                newEnemy.release();
            }
        }

        // --- Logic từng level ---
        if (currentLevel == 1) {
            processChickenVsPlayer(dt);

            if (chickenKillCount >= 18 && player.get_health() > 0) {
                currentLevel = 2;
                levelManager.setCurrentLevel(currentLevel);
                for (auto* c : chickens) delete c;
                chickens.clear();
                playMusicLevel(currentLevel);
                introBeforeLevel(dt);
            }
        }
        else if (currentLevel == 2) {
            processAsteroidVsPlayer(dt);

            if (allLevel2AsteroidDead() && player.get_health() > 0) {
                currentLevel = 3;
                levelManager.setCurrentLevel(currentLevel);
                for (auto* a : asteroids) delete a;
                asteroids.clear();
                playMusicLevel(currentLevel);
                introBeforeLevel(dt);
            }
        }
        else if (currentLevel == 3) {
            processBossVsPlayer(dt);

            if (allBossDead() && player.get_health() > 0) {
                currentLevel = 4;
                for (auto* b : bosses) delete b;
                bosses.clear();
                playMusicLevel(currentLevel);
            }
        }
        else if (player.get_health() <= 0 && playerWantToPlayAgain) {
            initGame();
            playerWantToPlayAgain = false;
            currentLevel = 1;
            levelManager.setCurrentLevel(currentLevel);
            playMusicLevel(currentLevel);
        }

        ui.update(score, player.get_health(), currentLevel);
        break;
    }

    case GameState::PAUSED:
        pauseMenu.update(dt);
        break;

    case GameState::SETTINGS:
        settingsMenu.update(dt);
        break;
    }
}

void App::render() {
    window.clear();
    background.render_background_scroll(window);

    switch (gameState) {
    case GameState::MENU:
        menu.render(window);
        break;

    case GameState::PLAYING:
        if (player.get_health() > 0) {
            player.render_animation(window, MAIN_OBJECT_SCALE);
            player.render_shooting(window);
        }

        for (auto* chicken : chickens) {
            if (chicken->get_is_on_screen() && chicken->get_health() > 0) {
                chicken->render_animation(window, CHICKEN_OBJECT_SCALE);
                chicken->render_the_eggs(window);
                if (chicken->get_has_a_present() && chicken->get_present()->get_is_on_screen())
                    chicken->get_present()->render(window);
            }
        }

        for (auto* asteroid : asteroids) {
            if (asteroid->get_is_on_screen()) asteroid->draw(window);
        }

        for (auto* boss : bosses) {
            if (boss->get_is_on_screen()) {
                boss->draw(window);
                boss->render_the_eggs(window);
                if (boss->get_has_a_present() && boss->get_present()->get_is_on_screen())
                    boss->get_present()->render(window);
            }
        }

        for (auto* exp : explosions) {
            if (exp->get_is_on_screen()) exp->draw(window);
        }

        ui.render(window);
        menu.render_time(window, &player);
        menu.render_health_bar(window, &player);

        if (player.get_health() <= 0) {
            menu.render_game_over(window, &player);
        }
        break;

    case GameState::PAUSED:
        player.render_animation(window, MAIN_OBJECT_SCALE);
        player.render_shooting(window);

        for (auto* chicken : chickens) {
            if (chicken->get_is_on_screen() && chicken->get_health() > 0) {
                chicken->render_animation(window, CHICKEN_OBJECT_SCALE);
                chicken->render_the_eggs(window);
                if (chicken->get_has_a_present() && chicken->get_present()->get_is_on_screen())
                    chicken->get_present()->render(window);
            }
        }

        for (auto* asteroid : asteroids) {
            if (asteroid->get_is_on_screen()) asteroid->draw(window);
        }

        for (auto* boss : bosses) {
            if (boss->get_is_on_screen()) {
                boss->draw(window);
                boss->render_the_eggs(window);
                if (boss->get_has_a_present() && boss->get_present()->get_is_on_screen())
                    boss->get_present()->render(window);
            }
        }

        for (auto* exp : explosions) {
            if (exp->get_is_on_screen()) exp->draw(window);
        }

        ui.render(window);
        pauseMenu.render(window);
        break;

    case GameState::SETTINGS:
        settingsMenu.render(window);
        break;
    }

    cursor.draw(window);
    window.display();
}

void App::initGame() {
    score = 0;
    lives = 3;
    chickenKillCount = 0;

    player.set_health(3);
    player.set_ammo_level(0);
    player.set_rect_cordinate(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    player.free();

    for (auto* c : chickens) delete c;
    chickens.clear();

    for (auto* a : asteroids) delete a;
    asteroids.clear();

    for (auto* e : explosions) delete e;
    explosions.clear();

    for (auto* p : presents) delete p;
    presents.clear();

    for (auto* b : bosses) delete b;
    bosses.clear();

    playerWantToPlayAgain = false;
    isPaused = false;
    std::cout << "Game initialized, level " << currentLevel << " started" << std::endl;
}

void App::initPlayer() {
    player.load_animation_sprite("res/image/ship.png");
    player.set_clips();
    player.set_rect_cordinate(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    player.set_health(5);
    player.set_ammo_level(0);
}

void App::initChickenLevel1() {
    chickens.clear();
    const int chicken_number = 18;
    for (int j = 0; j < 3; ++j) {
        for (int i = (j * chicken_number) / 3; i < ((j + 1) * chicken_number) / 3; ++i) {
            Chicken* chicken = new Chicken;
            chicken->load_animation_sprite("res/image/chicken123.png");
            chicken->set_clips();
            chicken->set_rect_cordinate(100 + (i - (j * chicken_number / 3)) * 100, j * 100 - 100);
            chicken->set_rect_width_and_height(75, 68);
            chicken->set_is_on_screen(true);
            chicken->set_health(1);
            chicken->set_speed(3);
            chicken->generate_present();
            chickens.push_back(chicken);
        }
    }
    std::cout << "Initialized " << chickens.size() << " chickens for level 1" << std::endl;
}

void App::initAsteroid() {
    asteroids.clear();
    const int number_of_asteroid = 10;
    for (int i = 0; i < number_of_asteroid; ++i) {
        Asteroid* asteroid = new Asteroid;
        asteroid->set_rect_cordinate(rand() % SCREEN_WIDTH, -100 - (rand() % 500));
        asteroid->set_is_on_screen(true);
        asteroid->set_speed(2);
        asteroid->set_health(2); // Tăng health để khó chết hơn
        asteroids.push_back(asteroid);
    }
    std::cout << "Initialized " << asteroids.size() << " asteroids for level 2" << std::endl;
}

void App::initBoss() {
    bosses.clear();
    const int boss_number = 2;
    for (int i = 0; i < boss_number; ++i) {
        Boss* boss = new Boss;
        boss->load_animation_sprite("res/image/boss.png");
        boss->set_clips();
        boss->set_rect_cordinate(i == 0 ? 100 : 700, 100);
        boss->set_is_on_screen(true);
        boss->set_health(100);
        bosses.push_back(boss);
    }
    std::cout << "Initialized " << bosses.size() << " bosses for level 3" << std::endl;
}

void App::playMusicLevel(int level) {
    if (isPaused) return;
    backgroundMusic.stop();
    std::string music_file;
    switch (level) {
    case 0: music_file = "res/sound/MENU_THEME.wav"; break;
    case 1: music_file = "res/sound/level_1_theme.wav"; break;
    case 2: music_file = "res/sound/asteroid_level.wav"; break;
    case 3: music_file = "res/sound/boss_theme.wav"; break;
    case 4: music_file = "res/sound/game_win.wav"; break;
    default: return;
    }
    if (!backgroundMusic.openFromFile(music_file)) {
        std::cerr << "Failed to load music: " << music_file << std::endl;
    }
    else {
        backgroundMusic.setVolume(settingsMenu.getMasterVolume());
        backgroundMusic.play();
    }
}

bool App::allLevel1ChickenDead() {
    bool dead = true;
    for (const auto* chicken : chickens) {
        if (chicken->get_health() > 0 && chicken->get_is_on_screen()) {
            dead = false;
            std::cout << "Alive chicken found at (" << chicken->x << ", " << chicken->y << "), health: " << chicken->get_health() << std::endl;
        }
    }
    std::cout << "allLevel1ChickenDead() returned: " << dead << std::endl;
    return dead;
}

bool App::allLevel2AsteroidDead() {
    bool dead = true;
    std::cout << "Checking asteroids for dead:" << std::endl;
    for (const auto* asteroid : asteroids) {
        std::cout << "Asteroid health: " << asteroid->get_health() << ", is_on_screen: " << asteroid->get_is_on_screen() << std::endl;
        if (asteroid->get_health() > 0 && asteroid->get_is_on_screen()) {
            dead = false;
        }
    }
    std::cout << "allLevel2AsteroidDead() returned: " << dead << std::endl;
    return dead;
}

bool App::allBossDead() {
    bool dead = true;
    for (const auto* boss : bosses) {
        if (boss->get_health() > 0 && boss->get_is_on_screen()) {
            dead = false;
        }
    }
    return dead;
}

// ====== CHICKEN vs PLAYER ======
void App::processChickenVsPlayer(float dt) {
    bool hitThisFrame = false;

    for (auto* chicken : chickens) {
        if (isPaused || !chicken->get_is_on_screen() || chicken->get_health() <= 0)
            continue;

        // 1. Movement theo pattern
        //switch (chicken->getMovementPattern()) {
        //case CIRCLE:   chicken->moving_like_a_circle();   break;
        //case DIAGONAL: chicken->moving_diagonally();      break;
        //case PARABOLA: chicken->moving_parabola();        break;
        //default:       chicken->moving_back_and_forth();  break;
        //}
        chicken->moving_diagonally();
        // 2. Shooting
        chicken->render_the_eggs(window);
        chicken->handle_shooting_eggs_toward_player(&player);

        // 3. Player bắn trúng gà?
        player.process_shooting_if_hit_chicken(chicken);

        // 4. Gà chết
        if (chicken->get_health() <= 0) {
            chickenKillCount++;
            score += 10;

            auto* exp = new Explosion;
            exp->load_animation_sprite("res/image/explosion.png");
            exp->set_clips();
            exp->set_coordinates(chicken->x, chicken->y);
            exp->set_is_on_screen(true);
            explosions.push_back(exp);

            if (chicken->get_has_a_present()) {
                chicken->generate_present();
            }

            chicken->set_is_on_screen(false);
            continue;
        }

        // 5. Player bị gà/trứng đụng
        if (!hitThisFrame && player.get_health() > 0) {
            player.process_if_hit_by_chicken(chicken);
            player.process_if_hit_by_eggs(chicken);
            if (player.get_got_hit()) hitThisFrame = true;
        }

        // 6. Player ăn quà
        if (auto* present = chicken->get_present()) {
            present->update();
            if (player.processing_if_got_present(present)) {
                if (present->get_kind_of_present() == BonusType::LIFE) {
                    player.set_health(player.get_health() + 1);
                    score += 10;
                }
                else if (present->get_kind_of_present() == BonusType::SHIELD) {
                    player.set_ammo_level(player.get_ammo_level() + 1);
                    score += 5;
                }
                chicken->set_has_present(false);
            }
        }
    }
}


// ====== ASTEROID vs PLAYER ======
void App::processAsteroidVsPlayer(float dt) {
    bool hitThisFrame = false;

    for (auto* asteroid : asteroids) {
        if (isPaused || !asteroid->get_is_on_screen()) continue;

        // 1. Movement update
        asteroid->update();

        // 2. Check collision với Player
        if (!hitThisFrame && player.get_health() > 0) {
            player.process_if_hit_by_asteroid(asteroid);
            if (player.get_got_hit()) hitThisFrame = true;
        }

        // 3. Check player bullet hit asteroid
        player.process_shooting_if_hit_asteroid(asteroid);

        // 4. Asteroid nổ?
        if (asteroid->get_health() <= 0) {
            score += 5;

            Explosion* exp = new Explosion;
            exp->load_animation_sprite("res/image/explosion.png");
            exp->set_clips();
            exp->set_coordinates(asteroid->x, asteroid->y);
            exp->set_is_on_screen(true);
            explosions.push_back(exp);

            asteroid->set_is_on_screen(false);
        }
    }
}

// ====== BOSS vs PLAYER ======
void App::processBossVsPlayer(float dt) {
    bool hitThisFrame = false;

    for (auto* boss : bosses) {
        if (isPaused || !boss->get_is_on_screen() || player.get_health() <= 0) continue;

        // 1. Movement + Shooting
        boss->moving_toward_player(&player);
        boss->firing_eggs();
        boss->update();

        // 2. Boss hit Player?
        if (!hitThisFrame) {
            player.processing_if_hit_by_boss(boss);
            player.processing_if_hit_by_boss_egg(boss);
            if (player.get_got_hit()) hitThisFrame = true;
        }

        // 3. Player bắn trúng boss?
        player.process_shooting_if_hit_boss(boss);

        // 4. Boss chết?
        if (boss->get_health() <= 0) {
            score += 50;

            Explosion* exp = new Explosion;
            exp->load_animation_sprite("res/image/explosion.png");
            exp->set_clips();
            exp->set_coordinates(boss->x, boss->y);
            exp->set_is_on_screen(true);
            explosions.push_back(exp);

            if (boss->get_has_a_present()) {
                boss->generate_present();
            }

            boss->set_is_on_screen(false);
            continue;
        }

        // 5. Boss rơi quà?
        if (boss->get_has_a_present()) {
            Present* present = boss->get_present();
            present->update();

            if (player.processing_if_got_present(present)) {
                if (present->get_kind_of_present() == BonusType::LIFE) {
                    player.set_health(player.get_health() + 1);
                    score += 10;
                }
                else if (present->get_kind_of_present() == BonusType::SHIELD) {
                    player.set_ammo_level(player.get_ammo_level() + 1);
                    score += 5;
                }
                boss->set_has_present(false);
            }
        }
    }
}

void App::introBeforeLevel(float dt) {
    sf::Clock clock;
    while (clock.getElapsedTime().asMilliseconds() <= 5000 && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (!isPaused)
                player.handling_movement(event);
            player.handling_shooting(event);
        }
        window.clear();
        background.render_background_scroll(window);
        background.update_background_scroll();
        player.update();
        player.render_animation(window, MAIN_OBJECT_SCALE);
        player.render_shooting(window);
        menu.render_before_level(window, currentLevel);
        if (!isPaused)
            menu.render_time(window, &player);
        menu.render_health_bar(window, &player);
        ui.render(window);
        cursor.draw(window);
        window.display();
    }
}

void App::cleanUpExplosions() {
    for (size_t i = 0; i < explosions.size(); ) {
        if (!explosions[i]->get_is_on_screen()) {
            delete explosions[i];
            explosions.erase(explosions.begin() + i);
        }
        else {
            ++i;
        }
    }
    std::cout << "Explosions remaining: " << explosions.size() << std::endl;
}
void App::cleanUpChickens() {
    for (size_t i = 0; i < chickens.size(); ) {
        if (!chickens[i]->get_is_on_screen()) {
            delete chickens[i];
            chickens.erase(chickens.begin() + i);
        }
        else {
            ++i;
        }
    }
    std::cout << "Chickens cleaned, remaining: " << chickens.size() << ", chickenKillCount: " << chickenKillCount << std::endl;
}