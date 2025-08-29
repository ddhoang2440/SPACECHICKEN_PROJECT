#include "LevelManager.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>
#include <ctime>

LevelManager::LevelManager()
    : current_wave_(1), score(0), chickenKillCount(0),
    isPaused(false), asteroidRoundTime(45.f), asteroidRoundElapsed(0.f),
    round2Active(false), presentSpawnTimer(0.f), presentSpawnInterval(2.f)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}


//void LevelManager::spawnRandomPresent() {
//
//    static std::mt19937 rng(std::random_device{}());
//    std::uniform_int_distribution<int> kindDist(0, 5);
//    std::uniform_real_distribution<float> xDist(50.f, SCREEN_WIDTH - 50.f);
//    std::uniform_real_distribution<float> speedDist(80.f, 150.f); // tốc độ rơi (pixel/sec)
//
//    float x = xDist(rng);
//    float y = -50.f; // xuất hiện trên màn hình
//    BonusType kind = static_cast<BonusType>(kindDist(rng));
//
//    auto present = std::make_unique<Present>(kind, x, y);
//    present->set_is_on_screen(true);
//    //present->set_speed(speedDist(rng)); // set tốc độ rơi
//
//    presents.push_back(std::move(present));
//}

// --------------------- Round 1: Chicken -----------------------
void LevelManager::processChickenVsPlayer(float dt, MainObject& player, sf::RenderWindow& window) {
    //bool hitThisFrame = false;

    for (auto& chicken : chickens) {
        if (isPaused || !chicken->get_is_on_screen() || chicken->get_health() <= 0)
            continue;

        switch (chicken->get_movement_pattern()) {
        case MovementPattern::CIRCLE:   chicken->moving_like_a_circle(); break;
        case MovementPattern::DIAGONAL: chicken->moving_diagonally(); break;
        case MovementPattern::PARABOLA: chicken->moving_parabola(); break;
        case MovementPattern::UP_DOWN:  chicken->moving_up_and_down(); break;
        default:                        chicken->moving_back_and_forth(); break;
        }

        //chicken->render_the_eggs(window);
        if (chicken->get_shooting_mode() == ShootingMode::DOWNWARD)
            chicken->handle_shooting_eggs_downward(dt);
        else
            chicken->handle_shooting_eggs_toward_player(&player, dt);
    }

    // Va chạm đạn với gà
    for (auto* ammo : player.get_ammo_list()) {
        if (!ammo->is_alive()) continue;

        for (auto& chicken : chickens) {
            if (!chicken->get_is_on_screen() || chicken->get_health() <= 0) continue;

            player.process_shooting_if_hit_chicken(chicken.get());
            if (!ammo->is_alive()) break;
        }
    }

    // Kiểm tra gà chết, explosion, điểm
    for (auto& chicken : chickens) {
        if (!chicken->get_is_on_screen() || chicken->get_health() > 0) continue;

        chickenKillCount++;
        score += 10;

        auto exp = std::make_unique<Explosion>();
        exp->load_animation_sprite("res/image/explosion.png");
        exp->set_clips();
        exp->set_coordinates(chicken->x, chicken->y);
        exp->set_is_on_screen(true);
        explosions.push_back(std::move(exp));
        if (Present* present = chicken->generate_present()) {
            std::cout << "Generated present for chicken at (" << chicken->x << ", " << chicken->y << ")" << std::endl;
            add_present(present);
        }
        chicken->set_is_on_screen(false);
    }

    // Gà tấn công player
    for (auto& chicken : chickens) {
        if (chicken->get_is_on_screen() && chicken->get_health() > 0 && player.get_health() > 0) {
            player.process_if_hit_by_chicken(chicken.get());
            player.process_if_hit_by_eggs(chicken.get());
        }
    }

    // Update presents
    for (auto it = presents.begin(); it != presents.end(); ) {
        Present* present = it->get();
        if (present->get_is_on_screen()) {
            present->update(dt);
            player.processing_if_got_present(present);
        }

        if (!present->get_is_on_screen())
            it = presents.erase(it);
        else
            ++it;
    }

    // Nếu tất cả gà chết → chuyển wave2
    bool all_dead = true;
    for (auto& chicken : chickens) {
        if (chicken->get_health() > 0 && chicken->get_is_on_screen()) {
            all_dead = false;
            break;
        }
    }

    if (all_dead && current_wave_ == 1) {
        cleanUpWave();
        //spawn_wave2(player);
		spawn_round3_wave2(player);
    }
}

// --------------------- Round 2: Asteroid -----------------------
// --------------------- Spawn Asteroids -----------------------
void LevelManager::spawnAsteroids(int num) {
//    const int maxAsteroidsOnScreen = 8;
//    if (asteroids.size() + num > maxAsteroidsOnScreen) {
//        num = maxAsteroidsOnScreen - static_cast<int>(asteroids.size());
//        if (num <= 0) return;
//    }
//
//    for (int i = 0; i < num; ++i) {
//        auto asteroid = std::make_unique<Asteroid>();
//
//        float x = static_cast<float>(rand() % SCREEN_WIDTH);
//        float y = -50.f - static_cast<float>(rand() % 200);
//        asteroid->set_rect_cordinate(x, y);
//
//
//        asteroid->set_speed(30.f); // 50-100
//
//        int size_level = rand() % 3; // 0=small,1=medium,2=large
//        asteroid->set_size_level(size_level);
//
//        // Gán health theo size
//        switch (size_level) {
//        case 0: asteroid->set_health(5); break;   // nhỏ
//        case 1: asteroid->set_health(10); break;  // vừa
//        case 2: asteroid->set_health(20); break;  // lớn
//        }
//
//        asteroid->set_is_on_screen(true);
//        asteroids.push_back(std::move(asteroid));
//    }
}

void LevelManager::updateRound2(float dt, MainObject& player) {
    //if (!round2Active) return;

    //asteroidRoundElapsed += dt;

    //// Spawn chậm
    //static float spawnTimer = 0.f;
    //spawnTimer += dt;

    //const int MAX_ASTEROID = 10; // số tối đa trên màn hình

    //if (spawnTimer >= 1.5f) { // spawn mỗi 1.5 giây
    //    int toSpawn = 1 + rand() % 2; // spawn 1-2 asteroid mỗi lần
    //    if (asteroids.size() + toSpawn > MAX_ASTEROID)
    //        toSpawn = MAX_ASTEROID - asteroids.size();

    //    if (toSpawn > 0)
    //        spawnAsteroids(toSpawn);

    //    spawnTimer = 0.f; // reset timer
    //}

    //// Update asteroid & xử lý va chạm
    //processAsteroidVsPlayer(dt, player);

    //// Remove asteroid chết hoặc ra ngoài màn hình
    //for (auto it = asteroids.begin(); it != asteroids.end(); ) {
    //    if (!(*it)->get_is_on_screen() || (*it)->get_health() <= 0)
    //        it = asteroids.erase(it);
    //    else
    //        ++it;
    //}

    //// Hết thời gian round → wave3
    //if (asteroidRoundElapsed >= asteroidRoundTime) {
    //    round2Active = false;
    //    asteroidRoundElapsed = 0.f;
    //    asteroids.clear();
    //    spawn_wave3(player);
    //}
}

void LevelManager::processAsteroidVsPlayer(float dt, MainObject& player) {
    bool hitThisFrame = false;

    for (auto& asteroid : asteroids) {
        if (isPaused || !asteroid->get_is_on_screen()) continue;

        asteroid->update();
        if (!hitThisFrame && player.get_health() > 0) {
            player.process_if_hit_by_asteroid(asteroid.get());
            if (player.get_got_hit()) hitThisFrame = true;
        }
        player.process_shooting_if_hit_asteroid(asteroid.get());

        if (asteroid->get_health() <= 0) {
            score += 5;
            auto exp = std::make_unique<Explosion>();
            exp->load_animation_sprite("res/image/explosion.png");
            exp->set_clips();
            exp->set_coordinates(asteroid->x, asteroid->y);
            exp->set_is_on_screen(true);
            explosions.push_back(std::move(exp));

            asteroid->set_is_on_screen(false);
        }
    }
}
void LevelManager::updateRound3(float dt, MainObject& player) {
    if (bosses.empty()) {
        processChickenVsPlayer(dt, player, window);
    }else
	processBossVsPlayer(dt, player, window);
}
// --------------------- Round 3: Boss -----------------------
void LevelManager::processBossVsPlayer(float dt, MainObject& player, sf::RenderWindow& window) {
    bool hitThisFrame = false;

    for (auto& boss : bosses) {
        if (isPaused || !boss->get_is_on_screen() || player.get_health() <= 0) {
            continue;
        }

        boss->moving_toward_player(&player);
        boss->firing_eggs();
        auto& egg_list = boss->get_egg_list();
        for (auto it = egg_list.begin(); it != egg_list.end();) {
            if ((*it)->is_off_screen()) {
                it = egg_list.erase(it);
            }
            else {
                (*it)->update(dt);
                (*it)->draw(window);
                ++it;
            }
        }
        
        boss->update(dt);

        if (!hitThisFrame) {
            player.processing_if_hit_by_boss(boss.get());
            player.processing_if_hit_by_boss_egg(boss.get());
            if (player.get_got_hit()) hitThisFrame = true;
        }

        player.process_shooting_if_hit_boss(boss.get());

        if (boss->get_health() <= 0) {
            score += 50;
            auto exp = std::make_unique<Explosion>();
            exp->load_animation_sprite("res/image/explosion.png");
            exp->set_clips();
            exp->set_coordinates(boss->x, boss->y);
            exp->set_is_on_screen(true);
            explosions.push_back(std::move(exp));
            boss->set_is_on_screen(false);
            continue;
        }

        if (boss->get_has_a_present()) {
            if (Present* present = boss->get_present()) {
                if (present->get_is_on_screen()) {
                    present->update(dt);
                    player.processing_if_got_present(present);
                }
            }
        }
    }
}

// --------------------- Render tất cả -----------------------
void LevelManager::render(sf::RenderWindow& window) {
    for (auto& c : chickens) {
        if (c->get_is_on_screen()) {
            c->draw(window);
			c->render_the_eggs(window);
        }
    }
    for (auto& a : asteroids)  if (a->get_is_on_screen()) a->draw(window);
    for (auto& b : bosses) {
        if (b->get_is_on_screen()) {
            b->draw(window);
			b->render_the_eggs(window);
        }
    }
    for (auto& p : presents)   if (p->get_is_on_screen()) p->draw(window);
    for (auto& e : explosions) if (e->get_is_on_screen()) e->draw(window);
}

// --------------------- Update chung -----------------------
void LevelManager::update(float dt, MainObject& player, sf::RenderWindow& window) {
    if (isPaused) return;
    player.update(dt);
    for (auto& ch : chickens) {
        if (ch->get_is_on_screen()) {
            ch->update(dt);
        }
    }
     
    //// Spawn vật phẩm ngẫu nhiên
    //presentSpawnTimer += dt;
    //if (presentSpawnTimer >= presentSpawnInterval) {
    //    presentSpawnTimer = 0.f;
    //    spawnRandomPresent();
    //}

    /*if (current_wave_ == 1) processChickenVsPlayer(dt, player, window);
    if (current_wave_ == 2) updateRound2(dt, player);*/
    if (current_wave_ == 1) updateRound3(dt, player);

    for (auto it = explosions.begin(); it != explosions.end(); ) {
        auto& e = *it;
        if (e->get_is_on_screen()) e->update();
        if (!e->get_is_on_screen()) it = explosions.erase(it);
        else ++it;
    }
}

// --------------------- Wave spawn -----------------------
void LevelManager::spawn_wave1() {
    //current_wave_ = 1;
    //chickens.clear();

    //int rows = 3;
    //int cols = 4;
    //float spacing_x = SCREEN_WIDTH / (cols + 1.f);
    //float spacing_y = 100.f;
    //float start_y = 50.f;
    //float frameWidth = 75.f;
    //float frameHeight = 68.f;

    //for (int row = 0; row < rows; ++row) {
    //    for (int col = 0; col < cols; ++col) {
    //        auto chicken = std::make_unique<Chicken>();
    //        float init_x = spacing_x * (col + 1);
    //        float init_y = start_y + spacing_y * row;

    //        chicken->set_rect_cordinate(init_x, init_y);
    //        chicken->load_animation_sprite("res/image/chicken123.png");
    //        chicken->set_clips();
    //        chicken->set_rect_width_and_height(frameWidth, frameHeight);
    //        chicken->set_is_on_screen(true);
    //        chicken->set_health(3);
    //        chicken->set_speed(2.f);
    //        chicken->set_y_direction(1.f);
    //        chicken->set_y_bounds(init_y - 50.f, init_y + 50.f);
    //        chicken->set_movement_pattern(MovementPattern::UP_DOWN);
    //        chicken->set_shooting_mode(ShootingMode::DOWNWARD);

    //        chickens.push_back(std::move(chicken));
    //    }
    //}

    //std::cout << "Spawned wave 1: " << chickens.size() << " chickens" << std::endl;
}

// --------------------- Wave 2: Asteroid -----------------------
void LevelManager::spawn_wave2(MainObject& player) {
    //current_wave_ = 2;
    //round2Active = true;
    //asteroidRoundElapsed = 0.f;
    //asteroids.clear();

    //// Không spawn ngay nhiều asteroid nữa
    //// spawnAsteroids(5 + rand() % 4);

    //std::cout << "Wave 2 started: dodge asteroids for " << asteroidRoundTime << " seconds" << std::endl;
}

//--------------------- Wave 3: Boss -----------------------
void LevelManager::spawn_round3_wave1() {
    /*current_wave_ = 1;
    bosses.clear();

    auto boss = std::make_unique<Boss>();
    boss->set_rect_cordinate(SCREEN_WIDTH / 2.f, 50.f);
    boss->set_health(100);
    boss->set_is_on_screen(true);
    bosses.push_back(std::move(boss));

    std::cout << "Spawned wave 3: Boss fight!" << std::endl;*/
     chickens.clear();
     const int rows = 3;
     const int cols = 5;
        float spacing_x = SCREEN_WIDTH / (cols + 1.f);
        float spacing_y = 100.f;
        float start_y = 50.f;
        float slight_range = 50.f;
        float speed = 2.f;
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                auto* chicken = new Chicken();
                float init_x = spacing_x * (col + 1);
                float init_y = start_y + spacing_y * row;
                chicken->set_rect_cordinate(init_x, init_y);
                chicken->load_animation_sprite("res/image/chicken123.png");
                chicken->set_clips();
                chicken->set_is_on_screen(true);
                chicken->set_health(3);
                chicken->set_speed(speed);
                chicken->set_y_direction(1.f);
                chicken->set_y_bounds(init_y - slight_range, init_y + slight_range);
                chicken->set_movement_pattern(MovementPattern::DIAGONAL);
                chicken->set_shooting_mode(ShootingMode::DOWNWARD);
                chickens.push_back(std::unique_ptr<Chicken>(chicken));
            }
        }
        std::cout << "Spawned wave 1: " << chickens.size() << " chickens" << std::endl;
}
void LevelManager::spawn_round3_wave2(MainObject& player) {
     //bosses.clear();
     //Boss* boss = new Boss;
     //boss->load_animation_sprite("res/image/bossMain.png");
     //boss->set_clips();
     //boss->set_rect_cordinate(SCREEN_WIDTH / 2, -200); // Xuất hiện từ trên
     //boss->setTargetPosition(SCREEN_WIDTH / 2, 150); // Di chuyển đến vị trí chiến đấu
     //boss->set_is_on_screen(true);
     //boss->set_health(150); // Có thể tăng máu
     //bosses.push_back(std::unique_ptr<Boss>(boss));
     //// Có thể play âm thanh boss xuất hiện
     //std::cout << "Boss spawned for level 3!" << std::endl;
    const int boss_number = 1;
    for (int i = 0; i < boss_number; ++i) {
        Boss* boss = new Boss;
        boss->load_animation_sprite("res/image/boss.png");
        boss->set_clips();
        /*boss->set_rect_cordinate(i == 0 ? 100 : 700, 100);*/
        boss->set_rect_cordinate(SCREEN_WIDTH / 2, -200); // Xuất hiện từ trên
        boss->setTargetPosition(SCREEN_WIDTH / 2, 150); // Di chuyển đến vị trí chiến đấu
        boss->set_is_on_screen(true);
        boss->set_health(50);
        bosses.push_back(std::unique_ptr<Boss>(boss));
    }
    std::cout << "Initialized " << bosses.size() << " bosses for level 3" << std::endl;
}
// --------------------- Cleanup -----------------------
void LevelManager::cleanUpChickens() {
    chickens.clear();
    chickenKillCount = 0;
}

// --------------------- Reset -----------------------
void LevelManager::reset() {
    chickens.clear();
    asteroids.clear();
    bosses.clear();
    presents.clear();
    explosions.clear();

    current_wave_ = 1;
    isPaused = false;
    score = 0;
    chickenKillCount = 0;
    round2Active = false;
    asteroidRoundElapsed = 0.f;
}

void LevelManager::cleanUpWave() {
    chickens.clear();
    asteroids.clear();
    bosses.clear();
    explosions.clear();
    presents.clear(); // <== thêm dòng này
}