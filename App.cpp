#include "App.h"
#include <iostream>

App::App()
    : window(sf::VideoMode::getDesktopMode(), "Space Chicken", sf::Style::Fullscreen),
    ui(), menu(window.getSize().x, window.getSize().y),
    pauseMenu(window.getSize().x, window.getSize().y),
    settingsMenu(window.getSize().x, window.getSize().y),
    cursor(), background(),
    player(), levelManager(),
    gameState(GameState::MENU),
    waitingForEnterEffect(false), enterEffectTime(0.f),
    isPaused(false),
    score(0), lives(3), currentLevel(1)
{
    window.setFramerateLimit(60);    
    window.setMouseCursorVisible(false);
    cursor.loadFromFile("assets/cursor/crosshair.png");

    initPlayer();
    background.loading_background("assets/images/background.png");
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
        if (event.type == sf::Event::Closed)
            window.close();

        // Lấy vị trí chuột
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        switch (gameState) {

        case GameState::MENU:
            menu.handleEvent(event, window);

            // Nhấn chuột trái trên menu
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int clicked = menu.handleMouseClick(mousePos);
                if (clicked != -1) {
                    switch (clicked) {
                    case 0: // New Game
                        gameState = GameState::PLAYING;
                        initGame(); // <-- gọi initGame() để spawn wave 1
                        break;
                    case 1: // Settings
						gameState = GameState::SETTINGS;
                        settingsMenu.show();
                        break;
                    case 2: // High Scores
                        gameState = GameState::HIGH_SCORES;
                        break;
                    case 3: // Help
                        gameState = GameState::HELP;

					case 4: // Quit
						window.close();
                        break;
                    }
                }
            }

            // Nhấn Enter để chọn
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                menu.activate();
            }

            // Nhấn Esc để thoát
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            break;

        case GameState::PAUSED:
            pauseMenu.handleEvent(event, window);

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int clicked = pauseMenu.handleMouseClick(mousePos);
                if (clicked == 0) { // Resume
                    pauseMenu.hide();
                    gameState = GameState::PLAYING;
                }
                else if (clicked == 1) { // Main Menu
                    pauseMenu.hide();
                    gameState = GameState::MENU;
                }
                else if (clicked == 2) { // Quit
                    window.close();
                }
            }

            // Nhấn Esc để resume
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                pauseMenu.hide();
                gameState = GameState::PLAYING;
            }
            break;

        case GameState::SETTINGS:
            settingsMenu.handleEvent(event, window);

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int clicked = settingsMenu.handleMouseClick(mousePos);
                if (clicked == static_cast<int>(settingsMenu.getOptionCount() - 1)) { // Back button
                    settingsMenu.hide();
                    gameState = GameState::MENU;
                }
            }

            // Nhấn Esc để quay lại menu
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                settingsMenu.hide();
                gameState = GameState::MENU;
            }
            break;

        case GameState::PLAYING:
            // Pause
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                gameState = GameState::PAUSED;
                pauseMenu.show();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (ui.getPauseButton().getGlobalBounds().contains(mousePos)) {
                    gameState = GameState::PAUSED;
                    pauseMenu.show();
                }
            }
            if (!isPaused && player.get_health() > 0) {
                player.handling_movement(event);
                player.handling_shooting(event);
            }
            break;

        case GameState::HIGH_SCORES:
            // TODO: xử lý sự kiện high scores
            break;

        case GameState::HELP:
            // TODO: xử lý sự kiện help screen
            break;

        case GameState::GAMEOVER:
            // Nhấn Enter để chơi lại
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                gameState = GameState::PLAYING;
                score = 0; lives = 3;
                levelManager.reset();
            }

            // Nhấn Esc để thoát
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            break;
        }
    }
}


void App::update(float dt) {
    // Ẩn/hiện con trỏ hệ thống và sprite dựa vào gameState
    if (gameState == GameState::PLAYING) {
        window.setMouseCursorVisible(false); // ẩn chuột hệ thống
        cursor.setVisible(false);            // ẩn sprite chuột
    }
    else {
        window.setMouseCursorVisible(true);  // hiện chuột hệ thống
        cursor.setVisible(true);             // hiện sprite chuột
    }

    // Cập nhật vị trí con trỏ sprite
    cursor.update(window);

    switch (gameState) {
    case GameState::MENU:
        menu.update(dt);
        if (waitingForEnterEffect) {
            enterEffectTime += dt;
            if (enterEffectTime >= MENU_ENTER_EFFECT_DURATION) {
                // Xử lý menu selection
                switch (menu.getSelectedIndex()) {
                case 0: // New Game
                    gameState = GameState::PLAYING;
                    levelManager.reset();
                    break;
                case 1: // Quit
                    window.close();
                    break;
                case 2: // High Scores
                    gameState = GameState::HIGH_SCORES;
                    // TODO: Show HighScoresMenu
                    break;
                case 3: // Settings
                    gameState = GameState::SETTINGS;
                    settingsMenu.show(); // Show settings menu
                    break;
                case 4: // Help
                    gameState = GameState::HELP;
                    // TODO: Show HelpMenu
                    break;
                }
                waitingForEnterEffect = false;
            }
        }
        break;

    case GameState::PLAYING:
        levelManager.update(dt, player, window);
        ui.update(score, lives, levelManager.getCurrentWave());

        if (player.get_health() <= 0) {
            lives--;                // giảm mạng
            if (lives > 0) {
                player.set_health(3);               // reset máu
                player.set_position(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f); // reset vị trí
            }
            else {
                gameState = GameState::GAMEOVER;   // hết mạng thì game over
            }
        }
        break;
    case GameState::PAUSED:
        pauseMenu.update(dt);
        break;

    case GameState::SETTINGS:
        settingsMenu.update(dt);
        backgroundMusic.setVolume(settingsMenu.getMasterVolume());
        if (settingsMenu.getSoundEffectsEnabled()) {
            if (backgroundMusic.getStatus() != sf::Music::Playing)
                backgroundMusic.play();
        }
        else {
            backgroundMusic.stop();
        }
        break;

    case GameState::HIGH_SCORES:
        // TODO: update high scores
        break;

    case GameState::HELP:
        // TODO: update help screen
        break;

    case GameState::GAMEOVER:
        // TODO: update game over screen
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
    case GameState::PAUSED:
        if (player.get_health() > 0) {
            player.render_animation(window, MAIN_OBJECT_SCALE);
            player.render_shooting(window);
        }

        // Render toàn bộ entities trong LevelManager
        levelManager.render(window);

        ui.render(window);

        if (gameState == GameState::PAUSED)
            pauseMenu.render(window);
        break;

    case GameState::SETTINGS:
        settingsMenu.render(window);
        break;

    case GameState::GAMEOVER: {
        sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        sf::Text text;
        text.setFont(gameOverFont);
        text.setString("GAME OVER\nPress Enter to Play Again\nPress Esc to Quit");
        text.setCharacterSize(60);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        text.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
        window.draw(text);
        break;
    }
    }

    cursor.draw(window);
    window.display();
}

void App::initGame() {
    levelManager.reset();
    player.set_health(3);
    score = 0;
    lives = 3;

    player.set_position(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f);

    // Spawn wave đầu tiên
    levelManager.spawn_wave1();
    gameState = GameState::PLAYING;
}

void App::initPlayer() {
    player.set_window(&window);
    player.load_animation_sprite("res/image/ship.png");
    player.set_clips();
    player.set_rect_cordinate(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100);
    player.set_health(3);
    player.set_ammo_level(0);
    window.setMouseCursorVisible(false);
}
