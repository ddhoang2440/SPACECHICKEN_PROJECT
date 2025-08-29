#include "MainObject.h"
#include <memory>
#include <iostream>
#include <cmath>

static sf::Clock MAIN_OBJECT_clock;
static int MAIN_OBJECT_spriteIndex = 0;
const sf::Time MAIN_OBJECT_spritetime = sf::milliseconds(100);

MainObject::MainObject()
    : Entity(), is_win_(false), spinning_angle_(0), width_of_sprite_(0), height_of_sprite_(0),
    health_(100), got_hit_(false), invincible(false), isColliding_(false), ammo_level_(0), number_of_wings_(0), slow_move_(false), ammo_type_(0)
{
    this->shots_ = 1;
	this->ammo_type_ = 3; // Mặc định là mũi tên
    x = SCREEN_WIDTH / 2;
    y = SCREEN_HEIGHT - 100;
    radius_ = 50; // Example collision radius
    slow_move_timer_ = MAIN_OBJECT_clock.getElapsedTime();

    if (!shoot_sound_buffer_.loadFromFile("res/sound/arrow_firing.wav"))
        std::cout << "Failed to load shoot sound" << std::endl;
    shoot_sound_.setBuffer(shoot_sound_buffer_);
    if (!eat_wing_sound_buffer_.loadFromFile("res/sound/eat_wing.wav"))
        std::cout << "Failed to load eat wing sound" << std::endl;
    eat_wing_sound_.setBuffer(eat_wing_sound_buffer_);
    if (!hit_sound_buffer_.loadFromFile("res/sound/hit.wav"))
        std::cout << "Failed to load hit sound" << std::endl;
    hit_sound_.setBuffer(hit_sound_buffer_);
    if (!getting_present_sound_buffer_.loadFromFile("res/sound/present.wav"))
        std::cout << "Failed to load present sound" << std::endl;
    getting_present_sound_.setBuffer(getting_present_sound_buffer_);
}

MainObject::~MainObject()
{
    free();
}

void MainObject::load_animation_sprite(const std::string& file)
{
    if (!texture_.loadFromFile(file))
    {
        std::cout << "Failed to load texture: " << file << std::endl;
        return;
    }
    width_of_sprite_ = texture_.getSize().x / MAIN_OBJECT_NUMS_FRAME;
    height_of_sprite_ = texture_.getSize().y;
    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, MAIN_OBJECT_NUMS_FRAME, 0.1f);
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setOrigin(width_of_sprite_ / 2.f, height_of_sprite_ / 2.f);
}

void MainObject::set_clips()
{
    if (width_of_sprite_ > 0 && height_of_sprite_ > 0)
    {
        anim_.frames_.clear();
        for (int i = 0; i < MAIN_OBJECT_NUMS_FRAME; ++i)
            anim_.frames_.push_back(sf::IntRect(i * width_of_sprite_, 0, width_of_sprite_, height_of_sprite_));
        anim_.sprite_.setTextureRect(anim_.frames_[0]);
    }
}

sf::FloatRect MainObject::get_rect() const
{
    float scale = 0.7f; // giảm hitbox còn 70%
    return sf::FloatRect(
        x - (width_of_sprite_ * scale) / 2,
        y - (height_of_sprite_ * scale) / 2,
        width_of_sprite_ * scale,
        height_of_sprite_ * scale
    );
}
void MainObject::handle_pull(float dt)
{
    if (!being_pulled_ || !pull_to_center_) return;

    float dx = target_pos_.x - x;
    float dy = target_pos_.y - y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 1.f)
    {
        // Di chuyển player về giữa
        x += (dx / dist) * pull_speed_ * dt * 60.0f;
        y += (dy / dist) * pull_speed_ * dt * 60.0f;
        anim_.sprite_.setPosition(x, y);

        // Đồng bộ chuột với player trong quá trình kéo
        if (window) {
            sf::Mouse::setPosition(sf::Vector2i(static_cast<int>(x), static_cast<int>(y)), *window);
        }
    }
    else
    {
        // Đã về đến giữa, dừng kéo
        being_pulled_ = false;
        pull_to_center_ = false;

        // Đảm bảo chuột ở đúng vị trí giữa
        if (window) {
            sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), *window);
        }

        std::cout << "Player pulled to center completed" << std::endl;
    }
}
void MainObject::start_pull_to_center()
{
    being_pulled_ = true;
    pull_to_center_ = true; // <== BẬT CHẾ ĐỘ KÉO VỀ GIỮA
    target_pos_.x = SCREEN_WIDTH / 2.0f;
    target_pos_.y = SCREEN_HEIGHT / 2.0f;
    
    // Đặt chuột về giữa ngay lập tức để bắt đầu
    if (window) {
        sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), *window);
    }
    
    std::cout << "Starting pull to center" << std::endl;
}
//void MainObject::render_animation(sf::RenderWindow& window, const double& scale)
//{
//    if (health_ <= 0) return;
//    sf::Time currentTicks = MAIN_OBJECT_clock.getElapsedTime();
//    if (currentTicks - slow_move_timer_ >= MAIN_OBJECT_spritetime)
//    {
//        MAIN_OBJECT_spriteIndex = (MAIN_OBJECT_spriteIndex + 1) % MAIN_OBJECT_NUMS_FRAME;
//        slow_move_timer_ = currentTicks;
//        anim_.sprite_.setTextureRect(anim_.frames_[MAIN_OBJECT_spriteIndex]);
//    }
//    if (slow_move_)
//    {
//        slowly_move_from_bottom();
//        return;
//    }
//    anim_.sprite_.setPosition(x, y);
//    anim_.sprite_.setScale(scale, scale);
//    anim_.sprite_.setRotation(spinning_angle_ + 90);
//    window.draw(anim_.sprite_);
//}
void MainObject::render_animation(sf::RenderWindow& window, const double& scale) {
    if (health_ <= 0) return;

    if (anim_.frames_.empty()) {
        std::cerr << "LỖI: Vector anim_.frames_ trống! Không thể render animation.\n";
        return;
    }

    sf::Time currentTicks = MAIN_OBJECT_clock.getElapsedTime();
    if (currentTicks - slow_move_timer_ >= MAIN_OBJECT_spritetime) {
        MAIN_OBJECT_spriteIndex = (MAIN_OBJECT_spriteIndex + 1) % MAIN_OBJECT_NUMS_FRAME;
        if (MAIN_OBJECT_spriteIndex >= anim_.frames_.size()) {
            MAIN_OBJECT_spriteIndex = 0;
        }
        slow_move_timer_ = currentTicks;
        anim_.sprite_.setTextureRect(anim_.frames_[MAIN_OBJECT_spriteIndex]);
    }

    if (slow_move_) {
        slowly_move_from_bottom();
        return;
    }

    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setScale(scale, scale);
    anim_.sprite_.setRotation(spinning_angle_); // Chỉ dùng spinning_angle_ mà không +90
    window.draw(anim_.sprite_);
}

void MainObject::update(float dt)
{
    // Kiểm tra trạng thái bất tử
    if (invincible)
    {
        // Nếu đã qua 3 giây kể từ khi bật bất tử
        if (immunity_timer_.getElapsedTime().asSeconds() >= 3.0f)
        {
            invincible = false; // Hết bất tử
        }
    }
    for (auto it = ammo_list.begin(); it != ammo_list.end(); )
    {
        AmmoObject* bullet = *it;
        if (bullet->is_alive())
        {
            float speed = bullet->get_speed();
            float rotation = bullet->get_rotation();
            float rad = rotation * 3.1415926535f / 180.0f;
            float dx = speed*200 * std::sin(rad) * dt;
            float dy = -speed*200 * std::cos(rad) * dt;

            sf::Vector2f pos = bullet->get_sprite().getPosition();
            bullet->set_rect_cordinate(pos.x + dx, pos.y + dy);

            if (pos.y < 0) bullet->set_alive(false);

            ++it;
        }
        else
        {
            delete* it;
            it = ammo_list.erase(it);
        }
    }
    if (health_ <= 0) return;

    if (slow_move_)
    {
        slowly_move_from_bottom();
    }
    else
    {
        anim_.update();
    }
}


void MainObject::handling_movement(sf::Event& event) {
    if (health_ <= 0 || slow_move_ || is_paused) return;

    if (event.type == sf::Event::MouseMoved) {
        x = event.mouseMove.x;
        y = event.mouseMove.y;
        if (x < 0) x = 0;
        if (x > SCREEN_WIDTH) x = SCREEN_WIDTH;
        if (y < 0) y = 0;
        if (y > SCREEN_HEIGHT) y = SCREEN_HEIGHT;

        // Tính góc quay dựa trên vị trí chuột so với trung tâm tàu
        float dx = event.mouseMove.x - x;
        float dy = event.mouseMove.y - y;
        spinning_angle_ = atan2(dy, dx) * 180 / 3.14159; // Chuyển từ radian sang độ
        anim_.sprite_.setPosition(x, y);
    }
}

//void MainObject::handling_shooting(sf::Event& event)
//{
//    if (health_ <= 0 || is_paused) return;
//
//    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
//    {
//        shoot_sound_.play();
//        auto p_ammo = std::make_unique<ArrowAmmo>();
//        p_ammo->set_rect_cordinate(x, y - height_of_sprite_ / 2);
//        p_ammo->set_type(AmmoObject::AmmoType(ammo_type_));
//        p_ammo->set_speed(10 + ammo_level_ * 2);
//        p_ammo->set_alive(true);
//        ammo_list.push_back(p_ammo.release());
//    }
//}
//
//void MainObject::render_shooting(sf::RenderWindow& window)
//{
//    for (auto it = ammo_list.begin(); it != ammo_list.end();)
//    {
//        if (!(*it)->is_alive())
//        {
//            delete* it;
//            it = ammo_list.erase(it);
//        }
//        else
//        {
//            (*it)->update();
//            (*it)->draw(window);
//            ++it;
//        }
//    }
//}
void MainObject::handling_shooting(sf::Event& event)
{
    if (health_ <= 0 || slow_move_) return;

    // Shoot on Space or left mouse click with 0.4s cooldown
    if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left))
    {
        if (shoot_timer_.getElapsedTime().asSeconds() >= 0.4f)
        {
            // Map ammo_type to bullet properties
            std::string ammo_type_str;
            AmmoType bullet_type;
            float bullet_speed;
            int damage;

            switch (ammo_type_)
            {
            case 1:
                ammo_type_str = "NEUTRON";
                bullet_type = AmmoType::NEURON;
                bullet_speed = 6.5f;
                damage = 1 + ammo_level_;
                break;
            case 2:
                ammo_type_str = "BORON";
                bullet_type = AmmoType::BORON;
                bullet_speed = 8.0f;
                damage = 2 + ammo_level_;
                break;
            case 3:
                ammo_type_str = "arrow";
                bullet_type = AmmoType::ARROW;
                bullet_speed = 7.25f;
                damage = 1 + ammo_level_;
                break;
            default:
                std::cerr << "Error: Invalid ammo_type " << ammo_type_ << std::endl;
                return;
            }

            // Calculate base position (center of player sprite)
            /*float sprite_width = texture_.getSize().x / static_cast<float>(MAIN_OBJECT_NUMS_FRAME);
            float base_x = x + sprite_width / 2.0f;
            float base_y = y - 50.0f;*/
            float base_x = x;
			float base_y = y - height_of_sprite_ / 2;

            // Create bullets based on shots_
            for (int i = 0; i < shots_; ++i)
            {
                // Create bullet based on type
                AmmoObject* bullet = nullptr;
                switch (bullet_type)
                {
                case AmmoType::NEURON:
                    bullet = new NeuronAmmo();
                    break;
                case AmmoType::BORON:
                    bullet = new BoronAmmo();
                    break;
                case AmmoType::ARROW:
                    bullet = new ArrowAmmo();
                    break;
                default:
                    continue; // Skip invalid bullet types
                }

                // Load bullet texture
                std::string path_ammo = "res/image/" + ammo_type_str + (ammo_type_ != 3 ? std::to_string(ammo_level_) : "") + ".png";
                if (!bullet->load_static_ammo_picture(path_ammo))
                {
                    std::cerr << "Error: Could not load " << path_ammo << std::endl;
                    delete bullet;
                    continue;
                }
                else {
                    cout<< "Loaded ammo texture: " << path_ammo <<"damage: "<<damage<< std::endl;
                }

                // Set bullet properties
                bullet->set_alive(true);
                //bullet->set_can_move(true);
                bullet->set_speed(bullet_speed);
                bullet->set_damage(damage);

                // Set position and rotation based on number of shots
                if (shots_ == 1)
                {
                    bullet->set_rect_cordinate(base_x, base_y);
                }
                else if (shots_ == 2)
                {
                    bullet->set_rect_cordinate(base_x + (i == 0 ? 23.0f : -23.0f), base_y);
                }
                else if (shots_ == 3)
                {
                    if (i == 0)
                    {
                        bullet->set_rotation(20.0f);
                        bullet->set_rect_cordinate(base_x + 10.0f, base_y);
                    }
                    else if (i == 1)
                    {
                        bullet->set_rotation(-20.0f);
                        bullet->set_rect_cordinate(base_x - 10.0f, base_y);
                    }
                    else
                    {
                        bullet->set_rect_cordinate(base_x, base_y);
                    }
                }

                ammo_list.push_back(bullet);
            }

            // Play shoot sound
            if (shoot_sound_.getStatus() != sf::Sound::Playing)
            {
                shoot_sound_.play();
            }
            shoot_timer_.restart();
        }
    }
}

//void MainObject::render_shooting(sf::RenderWindow& window_,float deltaTime)
//{
//    if (health_ <= 0 || slow_move_) return;
//
//    for (auto it = ammo_list.begin(); it != ammo_list.end();)
//    {
//        AmmoObject* bullet = *it;
//        if (bullet->is_alive())
//        {
//            // Update bullet position based on rotation and speed
//            float speed = bullet->get_speed();
//            float rotation = bullet->get_rotation();
//            float rad = rotation * 3.1415926535f / 180.0f; // Convert to radians
//            float dx = speed * std::sin(rad) * deltaTime;
//            float dy = -speed * std::cos(rad) * deltaTime; // Negative for upward movement
//            sf::Vector2f pos = bullet->get_sprite().getPosition();
//            bullet->set_rect_cordinate(pos.x + dx, pos.y + dy);
//
//            // Check if bullet is off-screen
//            if (pos.y < 0)
//            {
//                bullet->set_alive(false);
//            }
//
//            // Render bullet
//            bullet->draw(window_);
//            ++it;
//        }
//        else
//        {
//            delete* it;
//            it = ammo_list.erase(it);
//        }
//    }
//}
void MainObject::render_shooting(sf::RenderWindow& window_)
{
    if (health_ <= 0 || slow_move_) return;

    for (auto* bullet : ammo_list)
    {
        if (bullet->is_alive())
            bullet->draw(window_);
    }
}

void MainObject::process_shooting_if_hit_chicken(Chicken* chicken) {
    if (!chicken->get_is_on_screen()) return;

    for (auto* ammo : ammo_list) {
        if (ammo->is_alive() && check_collision_2_rect(ammo->get_rect(), chicken->get_rect())) {
            // Giảm health của gà
            int currentHealth = chicken->get_health();
            if (currentHealth > 0) {
                chicken->set_health(currentHealth - 1); // Giảm health
                std::cout << "Hit chicken, health reduced to: " << chicken->get_health() << std::endl; // Debug
            }
        }
    }
}

void MainObject::process_shooting_if_hit_boss(Boss* boss) {
    if (!boss->get_is_on_screen()) return;

    for (auto* ammo : ammo_list) {
        if (ammo->is_alive() && check_collision_2_rect(ammo->get_rect(), boss->get_rect())) {
            // Giảm health của boss
            int currentHealth = boss->get_health();
            if (currentHealth > 0) {
                boss->set_health(currentHealth - 1); // Giảm 1 health mỗi lần bắn
                std::cout << "Hit boss, health reduced to: " << boss->get_health() << std::endl; // Debug
            }
        }
    }
}

void MainObject::process_if_eat_wing_rect(Chicken* chicken)
{
    if (check_collision_2_rect(get_rect(), chicken->get_rect()) && chicken->get_is_on_screen() && health_ > 0)
    {
        number_of_wings_++;
        eat_wing_sound_.play();
        chicken->set_is_on_screen(false);
    }
}
void MainObject::process_if_hit_by_chicken(Chicken* chicken)
{
    if (invincible) {
        // Nếu đang bất tử thì kiểm tra xem đã hết 1 giây chưa
        if (immunity_timer_.getElapsedTime().asSeconds() >= 1.0f) {
            invincible = false;
        }
        else {
            return; // Chưa hết thì bỏ qua va chạm
        }
    }

    if (check_collision_2_rect(get_rect(), chicken->get_rect()) && chicken->get_is_on_screen() && health_ > 0)
    {
        health_--;
        hit_sound_.play();
        got_hit_ = true;
        // Bật trạng thái bất tử và reset timer
        invincible = true;
        immunity_timer_.restart();
		chicken->set_is_on_screen(false);
    }
}

void MainObject::process_if_hit_by_eggs(Chicken* chicken)
{
    if (invincible) {
        // Nếu đang bất tử thì kiểm tra xem đã hết 1 giây chưa
        if (immunity_timer_.getElapsedTime().asSeconds() >= 1.0f) {
            invincible = false;
        }
        else {
            return; // Chưa hết thì bỏ qua va chạm
        }
    }

    for (auto* egg : chicken->get_eggs_list())
    {
        if (check_collision_2_rect(get_rect(), egg->get_rect()) && egg->get_alive() && health_ > 0)
        {
            health_--;
            hit_sound_.play();
            got_hit_ = true;
            // Bật trạng thái bất tử và reset timer
            invincible = true;
            immunity_timer_.restart();
            egg->set_alive(false);
            break; // Thoát vòng lặp sau khi bị hit bởi một quả trứng
        }
    }
}

void MainObject::processing_if_hit_by_boss_egg(Boss* boss)
{
    if (invincible) {
        // Nếu đang bất tử thì kiểm tra xem đã hết 1 giây chưa
        if (immunity_timer_.getElapsedTime().asSeconds() >= 1.0f) {
            invincible = false;
        }
        else {
            return; // Chưa hết thì bỏ qua va chạm
        }
    }

    for (auto* egg : boss->get_egg_list())
    {
        if (check_collision_2_rect(get_rect(), egg->get_rect()) && egg->get_alive() && health_ > 0)
        {
            health_--;
            hit_sound_.play();
            got_hit_ = true;
            // Bật trạng thái bất tử và reset timer
            invincible = true;
            immunity_timer_.restart();
            egg->set_alive(false);
            break; // Thoát vòng lặp sau khi bị hit bởi một quả trứng
        }
    }
}

void MainObject::processing_if_hit_by_boss(Boss* boss)
{
    if (invincible) {
        // Nếu đang bất tử thì kiểm tra xem đã hết 1 giây chưa
        if (immunity_timer_.getElapsedTime().asSeconds() >= 1.0f) {
            invincible = false;
        }
        else {
            return; // Chưa hết thì bỏ qua va chạm
        }
    }

    if (check_collision_2_rect(get_rect(), boss->get_rect()) && boss->get_is_on_screen() && health_ > 0)
    {
        health_--;
        hit_sound_.play();
        got_hit_ = true;
        // Bật trạng thái bất tử và reset timer
        invincible = true;
        immunity_timer_.restart();
    }
}

void MainObject::process_shooting_if_hit_asteroid(Asteroid* asteroid) {
    if (!asteroid || !asteroid->get_is_on_screen() || asteroid->get_health() <= 0) return;

    for (auto* ammo : ammo_list) {
        if (ammo && ammo->is_alive()) {
            sf::FloatRect ammoRect = ammo->get_rect();
            sf::FloatRect asteroidRect = asteroid->get_rect();

            if (ammoRect.intersects(asteroidRect)) {
                // Giảm health của asteroid
                asteroid->set_health(asteroid->get_health() - ammo->get_damage());
                std::cout << "Hit asteroid at (" << asteroid->x << ", " << asteroid->y << ") with damage " << ammo->get_damage() << ", health remaining: " << asteroid->get_health() << std::endl;

                // Tắt đạn sau khi va chạm
                ammo->set_alive(false);

                // Thêm explosion (tùy chọn, nếu muốn hiệu ứng)
                if (asteroid->get_health() <= 0) {
                    Explosion* exp = new Explosion;
                    exp->load_animation_sprite("res/image/explosion.png");
                    exp->set_clips();
                    exp->set_coordinates(asteroid->x, asteroid->y);
                    exp->set_is_on_screen(true);
                    explosion_list.push_back(exp);
                }
                break; // Thoát vòng lặp sau khi va chạm
            }
        }
    }
}

void MainObject::process_if_hit_by_asteroid(Asteroid* asteroid)
{
    if (invincible) {
        // Nếu đang bất tử thì kiểm tra xem đã hết 1 giây chưa
        if (immunity_timer_.getElapsedTime().asSeconds() >= 1.0f) {
            invincible = false;
        }
        else {
            return; // Chưa hết thì bỏ qua va chạm
        }
    }

    if (check_collision_2_rect(get_rect(), asteroid->get_rect()) && asteroid->get_is_on_screen() && health_ > 0)
    {
        health_--;
        hit_sound_.play();
        got_hit_ = true;
        // Bật trạng thái bất tử và reset timer
        invincible = true;
        immunity_timer_.restart();
        asteroid->set_is_on_screen(false);
    }
}

//bool MainObject::processing_if_got_present(Present* present)
//{
//    if (check_collision_2_rect(get_rect(), present->get_rect()) && present->get_is_on_screen() && health_ > 0)
//    {
//        getting_present_sound_.play();
//        ammo_level_++;
//        present->set_is_on_screen(false);
//        return true;
//    }
//    return false;
//}
void MainObject::processing_if_got_present(Present* present)
{
    // Check for invalid present or dead player
    if (!present || health_ <= 0) return;

    // Create player rectangle (centered, assuming x, y are the sprite's center)
    sf::FloatRect player_rect(x - width_of_sprite_ / 2.0f, y - height_of_sprite_ / 2.0f, width_of_sprite_, height_of_sprite_);

    // Check collision and if present is on screen
    if (present->get_is_on_screen() && check_collision_2_rect(present->get_rect(), player_rect))
    {
        // Play sound if valid
        if (getting_present_sound_.getStatus() != sf::Sound::Playing)
        {
            getting_present_sound_.play();
        }

        // Mark present as collected
        present->set_is_on_screen(false);

        // Handle effects based on present type
        switch (present->get_kind())
        {
        case ATOMIC_POWER:
            if (shots_ < 3) shots_++;
        case LIFE:
            if (health_ < 10)
            {
                health_++;
            }
            break;
        case NEUTRON:
            ammo_type_ = 1;
            if (ammo_level_ < 3) ammo_level_++;
            
            break;
        case BORON:
            ammo_type_ = 2;
            if (ammo_level_ < 3) ammo_level_++;
            break;
        case ARROW:
            ammo_type_ = 3;
            if (ammo_level_ < 3) ammo_level_++;
            break;
        case SHIELD:
            invincible = true;
            immunity_timer_.restart();
            break;
        default:
            std::cerr << "Warning: Unknown present type " << present->get_kind() << std::endl;
            break;
        }
    }
}
void MainObject::slowly_move_from_bottom()
{
    if (y < SCREEN_HEIGHT - height_of_sprite_ / 2)
    {
        y = SCREEN_HEIGHT + height_of_sprite_;
        slow_move_ = false;
        return;
    }
    y -= 5;
    anim_.sprite_.setPosition(x, y);
}

void MainObject::set_slow_move()
{
    slow_move_ = true;
    slow_move_timer_ = MAIN_OBJECT_clock.getElapsedTime();
}

void MainObject::free()
{
    for (auto* ammo : ammo_list)
        delete ammo;
    ammo_list.clear();
    for (auto* exp : explosion_list)
        delete exp;
    explosion_list.clear();
}

void MainObject::set_rect_cordinate(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
    anim_.sprite_.setPosition(x, y);
}

void MainObject::set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height)
{
    this->x = x;
    this->y = y;
    width_of_sprite_ = width;
    height_of_sprite_ = height;
    anim_ = Animation(texture_, 0, 0, width, height, MAIN_OBJECT_NUMS_FRAME, 0.1f);
    anim_.sprite_.setPosition(x, y);
}

void MainObject::set_rect_width_and_height(const float& width, const float& height)
{
    width_of_sprite_ = width;
    height_of_sprite_ = height;
    anim_ = Animation(texture_, 0, 0, width, height, MAIN_OBJECT_NUMS_FRAME, 0.1f);
}

sf::FloatRect MainObject::get_rect_width_height_with_scale(const double& scale) const
{
    return sf::FloatRect(x - width_of_sprite_ * scale / 2, y - height_of_sprite_ * scale / 2,
        width_of_sprite_ * scale, height_of_sprite_ * scale);
}