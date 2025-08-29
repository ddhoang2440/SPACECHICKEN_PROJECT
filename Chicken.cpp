#include "Chicken.h"
#include <cmath>
#include <random>
#include <iostream>
#define M_PI 3.14159265358979323846

const sf::Time Chicken::CHICKEN_spritetime = sf::milliseconds(50);
int Chicken::CHICKEN_spriteIndex = 0;

Chicken::Chicken()
    : Entity(), health_(2), speed_(10), egg_shooting_prob_(50), scale_(CHICKEN_OBJECT_SCALE),
    angle_(90), angle_increment_(2), is_on_screen_(true), has_wing_(rand() % 100 <= 70),
    has_present_(rand() % 100 <= 50), width_of_sprite_(0), height_of_sprite_(0),
    v_x_(4.0), v_y_(4.0), dir_x_(1), dir_y_(1), x_direction_(1), wing_fall_speed_(10),
    damage_(0), type_(1), direction_(0.0), movement_pattern_(MovementPattern::UP_DOWN)
{
    x = 0;
    y = 0;
    radius_ = 75 / 2; // Based on rect_.width = 75
	present_ = new Present(x, y);
    wing_rect_ = sf::FloatRect(0, 0, 59, 84);
    last_egg_time_ = sf::Time::Zero;
    broken_egg_time_ = sf::Time::Zero;
    last_wing_time_ = sf::Time::Zero;
    last_move_time_ = sf::Time::Zero;

    if (!texture_.loadFromFile("res/image/chicken123.png"))
        std::cout << "Failed to load chicken texture" << std::endl;
    if (!wing_texture_.loadFromFile("res/image/meat.png"))
        std::cout << "Failed to load wing texture" << std::endl;

    width_of_sprite_ = texture_.getSize().x / CHICKEN_NUMS_FRAME;
    height_of_sprite_ = texture_.getSize().y;
    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, CHICKEN_NUMS_FRAME, 0.05f);
    anim_.sprite_.setPosition(x, y);


    if (!chicken_got_hit_buffer_.loadFromFile("res/sound/CHICKEN_GOT_HIT.wav"))
        std::cout << "Failed to load chicken hit sound" << std::endl;
    if (!chicken_laying_eggs_buffer_.loadFromFile("res/sound/Laying_eggs.wav"))
        std::cout << "Failed to load laying eggs sound" << std::endl;
    if (!eggs_get_destroyed_buffer_.loadFromFile("res/sound/Egg_Destroy.wav"))
        std::cout << "Failed to load egg destroyed sound" << std::endl;

    chicken_got_hit_sound_.setBuffer(chicken_got_hit_buffer_);
    chicken_laying_eggs_sound_.setBuffer(chicken_laying_eggs_buffer_);
    eggs_get_destroyed_sound_.setBuffer(eggs_get_destroyed_buffer_);
}

Chicken::~Chicken()
{
    free();
}

void Chicken::set_clips()
{
    if (width_of_sprite_ > 0 && height_of_sprite_ > 0)
    {
        anim_.frames_.clear();
        for (int i = 0; i < CHICKEN_NUMS_FRAME; ++i)
            anim_.frames_.push_back(sf::IntRect(i * width_of_sprite_, 0, width_of_sprite_, height_of_sprite_));
        anim_.sprite_.setTextureRect(anim_.frames_[0]);
    }
}

void Chicken::load_animation_sprite(const std::string& file)
{
    if (!texture_.loadFromFile(file))
    {
        std::cout << "Failed to load texture: " << file << std::endl;
        return;
    }
    width_of_sprite_ = texture_.getSize().x / CHICKEN_NUMS_FRAME;
    height_of_sprite_ = texture_.getSize().y;
    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, CHICKEN_NUMS_FRAME, 0.05f);
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setOrigin(width_of_sprite_ / 2.f , height_of_sprite_ / 2.f);

}

void Chicken::set_rect_cordinate(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
    if (present_) {
        present_->set_rect_cordinate(x, y); // Sync present position
    }
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left   = x - width_of_sprite_  / 2.f;
    wing_rect_.top    = y - height_of_sprite_ / 2.f;
    wing_rect_.width  = width_of_sprite_;
    wing_rect_.height = height_of_sprite_;
}

void Chicken::set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height)
{
    this->x = x;
    this->y = y;
    width_of_sprite_ = width;
    height_of_sprite_ = height;
    anim_ = Animation(texture_, 0, 0, width, height, CHICKEN_NUMS_FRAME, 0.05f);
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - width_of_sprite_ / 2.f;
    wing_rect_.top = y - height_of_sprite_ / 2.f;
    wing_rect_.width = width_of_sprite_;
    wing_rect_.height = height_of_sprite_;
}

void Chicken::set_rect_width_and_height(const float& width, const float& height)
{
    width_of_sprite_ = width;
    height_of_sprite_ = height;
    anim_ = Animation(texture_, 0, 0, width, height, CHICKEN_NUMS_FRAME, 0.05f);
}

sf::FloatRect Chicken::get_rect() const
{
    return sf::FloatRect(x - width_of_sprite_ / 2, y - height_of_sprite_ / 2, width_of_sprite_, height_of_sprite_);
}

sf::FloatRect Chicken::get_rect_width_height_with_scale(const double& scale) const
{
    return sf::FloatRect(x - width_of_sprite_ * scale / 2, y - height_of_sprite_ * scale / 2,
        width_of_sprite_ * scale, height_of_sprite_ * scale);
}

void Chicken::render_animation(sf::RenderWindow& window, const double& scale)
{
    if (health_ > 0)
    {
        draw(window);
    }
    else if (has_wing_)
    {
        sf::Time currentTime = sf::Clock().getElapsedTime();
        if (currentTime - last_wing_time_ > sf::seconds(6))
        {
            has_wing_ = false;
            wing_rect_.left = -9999;
            wing_rect_.top = -9999;
            speed_ = 0;
            return;
        }
        if (wing_rect_.top + wing_rect_.height <= SCREEN_HEIGHT)
        {
            wing_rect_.top += wing_fall_speed_;
        }
        if (is_on_screen_)
        {
            sf::Sprite wing_sprite(wing_texture_);
            wing_sprite.setPosition(wing_rect_.left, wing_rect_.top);
            window.draw(wing_sprite);
        }
        else
        {
            wing_rect_.left = -9999;
            wing_rect_.top = -9999;
            speed_ = 0;
            wing_rect_.width = 0;
            wing_rect_.height = 0;
        }
    }
    // Render present if it exists and is on-screen
    if (present_ && present_->get_is_on_screen()) {
        present_->render(window); // Assuming Present has a render method
    }
}

void Chicken::draw(sf::RenderWindow& window)
{
    if (!is_on_screen_ || health_ <= 0) return;
    sf::Time currentTime = sf::Clock().getElapsedTime();
    if (currentTime - last_move_time_ >= CHICKEN_spritetime)
    {
        CHICKEN_spriteIndex = (CHICKEN_spriteIndex + 1) % CHICKEN_NUMS_FRAME;
        anim_.sprite_.setTextureRect(anim_.frames_[CHICKEN_spriteIndex]);
        last_move_time_ = currentTime;
    }
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setScale(scale_, scale_);
    window.draw(anim_.sprite_);
}

void Chicken::update(float dt)
{
    if (!is_on_screen_ || health_ <= 0) return;
    if (health_ <= 0) {
        generate_present(); // Call when chicken dies
        return;
    }
    anim_.update();
	update_eggs(dt); // Assuming 60 FPS, dt ~ 0.016s
    if (present_ && present_->get_is_on_screen()) {
        present_->update(dt);
    }
	
}
void Chicken::update_eggs(float dt)
{
    for (auto it = eggs_list_.begin(); it != eggs_list_.end();)
    {
        Egg* egg = *it;

        // CẬP NHẬT VỊ TRÍ TRỨNG
        float new_x = egg->get_x() + egg->get_v_x() * dt * 60.0f;
        float new_y = egg->get_y() + egg->get_v_y() * dt * 60.0f;
        egg->set_rect_cordinate(new_x, new_y);

        // Kiểm tra nếu trứng ra khỏi màn hình
        if (new_y > SCREEN_HEIGHT || !egg->get_alive())
        {
            delete egg;
            it = eggs_list_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
void Chicken::handle_shooting_eggs_downward(float dt)
{
    if (health_ <= 0 || !is_on_screen_) return;

    shoot_timer_ += dt;
    if (shoot_timer_ >= shoot_interval_)
    {
        // Xác suất bắn (50%)
        if (rand() % 100 < 50)
        {
            auto* egg = new Egg();

            // Tính vị trí trứng
            float egg_x = x;
            float egg_y = y + (height_of_sprite_ * scale_) + 5.0f; // Dưới chân gà

            egg->set_rect_cordinate(egg_x, egg_y);
			egg->set_v_x(0.0f);
			egg->set_v_y(4.0f + (rand() % 3));
            egg->set_alive(true);

            eggs_list_.push_back(egg);
            chicken_laying_eggs_sound_.play();
        }

        shoot_timer_ = 0.f;
    }
}
void Chicken::handle_shooting_eggs_toward_player(MainObject* main_object, float dt)
{
    if (!main_object || main_object->get_slow_move() || health_ <= 0 || !is_on_screen_) return;
    shoot_timer_ += dt;
    if (shoot_timer_ < shoot_interval_) return;

    int rand_num = rand() % 100 + 1;
    if (rand_num <= egg_shooting_prob_)
    {
        double dx = main_object->get_rect().left + main_object->get_rect().width / 2 - x;
        double dy = main_object->get_rect().top + main_object->get_rect().height / 2 - y;
        double distance = std::sqrt(dx * dx + dy * dy);
        if (distance > 0) // Tránh chia cho 0
        {
            double unit_x = dx / distance;
            double unit_y = dy / distance;
            double egg_speed = 9.0;
            Egg* egg = new Egg();
            egg->set_rect_cordinate_width_and_height(x, y, EGG_WIDTH, EGG_HEIGHT);
            egg->set_v_x(egg_speed * unit_x);
            egg->set_v_y(egg_speed * unit_y);
            egg->set_alive(true);
            eggs_list_.push_back(egg); // Sửa eggs_list_ thành eggs_list
            chicken_laying_eggs_sound_.play();
            shoot_timer_ = 0.f;
        }
    }
}

void Chicken::update_the_eggs(float dt)
{
    for (auto it = eggs_list_.begin(); it != eggs_list_.end();)
    {
        (*it)->update(dt);
        if (!(*it)->get_alive())
        {
            eggs_get_destroyed_sound_.play();
            delete* it;
            it = eggs_list_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Chicken::render_the_eggs(sf::RenderWindow& window)
{
    for (auto* egg : eggs_list_)
        egg->render(window);
}

void Chicken::play_hit_sound()
{
    chicken_got_hit_sound_.setPlayingOffset(sf::milliseconds(400));
    chicken_got_hit_sound_.play();
}

void Chicken::play_laying_eggs_sound()
{
    chicken_laying_eggs_sound_.play();
}

//void Chicken::generate_present()
//{
//    if (!has_present_) return;
//    if (health_ > 0)
//    {
//        present_->set_kind_of_present(static_cast<BonusType>(rand() % 6));
//        if (present_->get_kind_of_present() == ARROW)
//            present_->set_kind_of_present(BORON); // Avoid ARROW
//        present_->set_rect_cordinate(x, y);
//    }
//    else
//    {
//        present_->set_is_on_screen(true);
//    }
//}
Present* Chicken::generate_present()
{
    if (!has_present_) return nullptr; // Only generate if has_present_ is true
    // Ensure present_ is valid
    Present* present = new Present(x, y);

    // Only generate a present when the chicken is destroyed (health_ <= 0)
    // Use modern random number generator
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 5); // Adjust range based on BonusType enum

    // Set present type
    BonusType new_type = static_cast<BonusType>(dist(rng));
    present->set_kind_of_present(new_type);

    // Set position to chicken's current position
    present->set_rect_cordinate(x, y);

    // Make present visible and active
    present->set_is_on_screen(true);

    std::cout << "Present generated at (" << x << ", " << y << ") with type " << new_type << std::endl;
	has_present_ = false; // Ensure only one present is generated
    return present;
}
void Chicken::moving_diagonally()
{
    if (!is_on_screen_ || health_ <= 0) return;
    x += v_x_ * dir_x_;
    y += v_y_ * dir_y_;
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
    if (x < 0)
        dir_x_ = 1;
    else if (x + width_of_sprite_ > SCREEN_WIDTH)
        dir_x_ = -1;
    if (y < 0)
        dir_y_ = 1;
    else if (y + height_of_sprite_ > SCREEN_HEIGHT)
        dir_y_ = -1;
}

void Chicken::moving_parabola()
{
    if (!is_on_screen_ || health_ <= 0) return;
    double a = -0.005;
    double h = SCREEN_WIDTH / 2.0;
    double k = SCREEN_HEIGHT / 2.0;
    x += x_direction_ * speed_;
    y = a * std::pow(x - h, 2) + k;
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
    if (x <= 0 || x + width_of_sprite_ >= SCREEN_WIDTH)
        x_direction_ *= -1;
}
//void Chicken::moving_up_and_down()
//{
//    if (!is_on_screen_ || health_ <= 0) return;
//    y += y_direction_ * speed_;
//    anim_.sprite_.setPosition(x, y);
//    wing_rect_.left = x - wing_rect_.width / 2;
//    wing_rect_.top = y - wing_rect_.height / 2;
//    if (y <= 0 || y + height_of_sprite_ >= SCREEN_HEIGHT)
//        y_direction_ *= -1;
//}
void Chicken::moving_up_and_down()
{
    if (!is_on_screen_ || health_ <= 0) return;

    // Cập nhật vị trí theo hướng Y
    y += y_direction_ * speed_;

    // Nếu vượt quá bounds thì đảo chiều
    if (y <= min_y_ || y >= max_y_) {
        y_direction_ *= -1.f;
    }

    // Cập nhật sprite và wing_rect_
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
}

void Chicken::moving_back_and_forth()
{
    if (!is_on_screen_ || health_ <= 0) return;
    x += x_direction_ * speed_;
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
    if (x <= 0 || x + width_of_sprite_ >= SCREEN_WIDTH)
        x_direction_ *= -1;
}

//void Chicken::moving_like_a_circle()
//{
//    if (!is_on_screen_ || health_ <= 0) return;
//    angle_ += angle_increment_;
//    if (angle_ >= 360) angle_ -= 360;
//    x = SCREEN_WIDTH / 2 + radius_ * std::cos(angle_ * M_PI / 180);
//    y = SCREEN_HEIGHT / 2 + radius_ * std::sin(angle_ * M_PI / 180);
//    anim_.sprite_.setPosition(x, y);
//    wing_rect_.left = x - wing_rect_.width / 2;
//    wing_rect_.top = y - wing_rect_.height / 2;
//}
void Chicken::moving_like_a_circle()
{
    if (!is_on_screen_ || health_ <= 0) return;

    // Tăng góc
    angle_ += angle_increment_;
    if (angle_ >= 360.f) angle_ -= 360.f;

    // Tính toán vị trí theo hình tròn
    x = center_x_ + radius_ * std::cos(angle_ * M_PI / 180.f);
    y = center_y_ + radius_ * std::sin(angle_ * M_PI / 180.f);

    // Cập nhật sprite và wing_rect_
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
}

void Chicken::moving_toward_the_player(MainObject* main_object)
{
    if (!is_on_screen_ || health_ <= 0 || !main_object) return;
    double dx = main_object->get_rect().left + main_object->get_rect().width / 2 - x;
    double dy = main_object->get_rect().top + main_object->get_rect().height / 2 - y;
    double distance = std::sqrt(dx * dx + dy * dy);
    if (distance > 0)
    {
        x += (dx / distance) * speed_;
        y += (dy / distance) * speed_;
        anim_.sprite_.setPosition(x, y);
        wing_rect_.left = x - wing_rect_.width / 2;
        wing_rect_.top = y - wing_rect_.height / 2;
    }
}

void Chicken::moving_toward_the_player()
{
    moving_toward_the_player(nullptr); // Placeholder
}

void Chicken::free()
{
    for (auto* egg : eggs_list_)
        delete egg;
    eggs_list_.clear();
    /*if (present_)
    {
        delete present_;
        present_ = nullptr;
    }*/
    present_ = nullptr;
    has_present_ = false;
}
//#include "Chicken.h"
//#include <cmath>
//#include <iostream>
//#include <algorithm>
//
//const sf::Time Chicken::CHICKEN_spritetime = sf::milliseconds(50);
//int Chicken::CHICKEN_spriteIndex = 0;
//
//Chicken::Chicken()
//    : Entity(),
//    health_(2), max_health_(2), speed_(1),
//    egg_shooting_prob_(30), scale_(CHICKEN_OBJECT_SCALE),
//    is_on_screen_(true), has_wing_(rand() % 100 <= 30),
//    has_present_(rand() % 100 <= 20),
//    movement_pattern_(MovementPattern::BACK_FORTH),
//    shooting_mode_(ShootingMode::DOWNWARD)
//{
//    x = 0;
//    y = 0;
//    width_of_sprite_ = 0;
//    height_of_sprite_ = 0;
//    v_x_ = 1.0f;
//    v_y_ = 1.0f;
//    x_direction_ = 1;
//    y_direction_ = 1;
//    angle_ = 0.0;
//    angle_increment_ = 1.0;
//    damage_ = 1;
//    wing_fall_speed_ = 2;
//
//    circle_center_x_ = SCREEN_WIDTH / 2.0f;
//    circle_center_y_ = SCREEN_HEIGHT / 2.0f;
//    circle_radius_ = 200.0f;
//    min_y_ = 0;
//    max_y_ = SCREEN_HEIGHT;
//
//    present_ = std::make_unique<Present>(static_cast<BonusType>(rand() % 6));
//    wing_rect_ = sf::FloatRect(0, 0, 59, 84);
//
//    // Load textures
//    if (!texture_.loadFromFile("res/image/chicken123.png"))
//        std::cout << "Failed to load chicken texture" << std::endl;
//    if (!wing_texture_.loadFromFile("res/image/meat.png"))
//        std::cout << "Failed to load wing texture" << std::endl;
//
//    width_of_sprite_ = texture_.getSize().x / CHICKEN_NUMS_FRAME;
//    height_of_sprite_ = texture_.getSize().y;
//
//    // Load sounds
//    if (!chicken_got_hit_buffer_.loadFromFile("res/sound/CHICKEN_GOT_HIT.wav"))
//        std::cout << "Failed to load chicken hit sound" << std::endl;
//    if (!chicken_laying_eggs_buffer_.loadFromFile("res/sound/Laying_eggs.wav"))
//        std::cout << "Failed to load laying eggs sound" << std::endl;
//    if (!eggs_get_destroyed_buffer_.loadFromFile("res/sound/Egg_Destroy.wav"))
//        std::cout << "Failed to load egg destroyed sound" << std::endl;
//
//    chicken_got_hit_sound_.setBuffer(chicken_got_hit_buffer_);
//    chicken_laying_eggs_sound_.setBuffer(chicken_laying_eggs_buffer_);
//    eggs_get_destroyed_sound_.setBuffer(eggs_get_destroyed_buffer_);
//}
//
//Chicken::~Chicken()
//{
//    cleanup();
//}
//
//void Chicken::cleanup()
//{
//    eggs_list_.clear();
//}
//
//void Chicken::set_clips()
//{
//    if (width_of_sprite_ > 0 && height_of_sprite_ > 0)
//    {
//        anim_.frames_.clear();
//        for (int i = 0; i < CHICKEN_NUMS_FRAME; ++i)
//        {
//            anim_.frames_.push_back(sf::IntRect(i * width_of_sprite_, 0, width_of_sprite_, height_of_sprite_));
//        }
//        if (!anim_.frames_.empty())
//        {
//            anim_.sprite_.setTextureRect(anim_.frames_[0]);
//        }
//    }
//}
//
//void Chicken::load_animation_sprite(const std::string& file)
//{
//    if (!texture_.loadFromFile(file))
//    {
//        std::cout << "Failed to load texture: " << file << std::endl;
//        return;
//    }
//    width_of_sprite_ = texture_.getSize().x / CHICKEN_NUMS_FRAME;
//    height_of_sprite_ = texture_.getSize().y;
//
//    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, CHICKEN_NUMS_FRAME, 0.05f);
//    anim_.sprite_.setPosition(x, y);
//    anim_.sprite_.setScale(scale_, scale_);
//}
//
//void Chicken::set_position(float x, float y)
//{
//    this->x = x;
//    this->y = y;
//    anim_.sprite_.setPosition(x, y);
//}
//
//void Chicken::set_size(float width, float height)
//{
//    width_of_sprite_ = width;
//    height_of_sprite_ = height;
//}
//
//sf::FloatRect Chicken::get_rect() const
//{
//    float scaled_width = width_of_sprite_ * scale_;
//    float scaled_height = height_of_sprite_ * scale_;
//    return sf::FloatRect(x - scaled_width / 2, y - scaled_height / 2, scaled_width, scaled_height);
//}
//
//sf::FloatRect Chicken::get_scaled_rect(double scale) const
//{
//    float scaled_width = width_of_sprite_ * scale;
//    float scaled_height = height_of_sprite_ * scale;
//    return sf::FloatRect(x - scaled_width / 2, y - scaled_height / 2, scaled_width, scaled_height);
//}
//
//void Chicken::render(sf::RenderWindow& window)
//{
//    if (!is_on_screen_ || health_ <= 0) return;
//
//    // Update animation
//    sf::Time current_time = sf::Clock().getElapsedTime();
//    if (current_time - last_move_time_ >= CHICKEN_spritetime)
//    {
//        CHICKEN_spriteIndex = (CHICKEN_spriteIndex + 1) % CHICKEN_NUMS_FRAME;
//        anim_.sprite_.setTextureRect(anim_.frames_[CHICKEN_spriteIndex]);
//        last_move_time_ = current_time;
//    }
//
//    anim_.sprite_.setPosition(x, y);
//    window.draw(anim_.sprite_);
//}
//
//void Chicken::update(float dt)
//{
//    if (!is_on_screen_) return;
//
//    // Handle movement based on pattern
//    switch (movement_pattern_)
//    {
//    case MovementPattern::UP_DOWN:
//        move_up_down(dt);
//        break;
//    case MovementPattern::CIRCLE:
//        move_circle(dt);
//        break;
//    case MovementPattern::DIAGONAL:
//        move_diagonal(dt);
//        break;
//    case MovementPattern::PARABOLA:
//        move_parabola(dt);
//        break;
//    case MovementPattern::BACK_FORTH:
//        move_back_forth(dt);
//        break;
//    default:
//        move_back_forth(dt);
//        break;
//    }
//
//    // Update eggs
//    update_eggs(dt);
//
//    // Update present
//    if (has_present_ && present_)
//    {
//        present_->update();
//    }
//}
//
//void Chicken::set_circle_properties(float center_x, float center_y, float radius, float angle_speed)
//{
//    circle_center_x_ = center_x;
//    circle_center_y_ = center_y;
//    circle_radius_ = radius;
//    angle_increment_ = angle_speed;
//}
//
//// Movement pattern implementations
//void Chicken::move_up_down(float dt)
//{
//    y += y_direction_ * speed_ * dt * 60.0f;
//
//    if (y <= min_y_ || y + height_of_sprite_ * scale_ >= max_y_)
//    {
//        y_direction_ *= -1;
//    }
//
//    anim_.sprite_.setPosition(x, y);
//}
//
//void Chicken::move_circle(float dt)
//{
//    angle_ += angle_increment_ * dt;
//    if (angle_ >= 360.0f) angle_ -= 360.0f;
//
//    x = circle_center_x_ + circle_radius_ * std::cos(angle_ * M_PI / 180.0f);
//    y = circle_center_y_ + circle_radius_ * std::sin(angle_ * M_PI / 180.0f);
//
//    anim_.sprite_.setPosition(x, y);
//}
//
//void Chicken::move_diagonal(float dt)
//{
//    x += x_direction_ * speed_ * dt * 60.0f;
//    y += y_direction_ * speed_ * dt * 60.0f;
//
//    if (x <= 0 || x + width_of_sprite_ * scale_ >= SCREEN_WIDTH)
//        x_direction_ *= -1;
//    if (y <= 0 || y + height_of_sprite_ * scale_ >= SCREEN_HEIGHT)
//        y_direction_ *= -1;
//
//    anim_.sprite_.setPosition(x, y);
//}
//
//void Chicken::move_parabola(float dt)
//{
//    static float time = 0.0f;
//    time += dt;
//
//    x += x_direction_ * speed_ * dt * 60.0f;
//    y = circle_center_y_ + circle_radius_ * std::sin(time);
//
//    if (x <= 0 || x + width_of_sprite_ * scale_ >= SCREEN_WIDTH)
//        x_direction_ *= -1;
//
//    anim_.sprite_.setPosition(x, y);
//}
//
//void Chicken::move_back_forth(float dt)
//{
//    x += x_direction_ * speed_ * dt * 60.0f;
//
//    if (x <= 0 || x + width_of_sprite_ * scale_ >= SCREEN_WIDTH)
//        x_direction_ *= -1;
//
//    anim_.sprite_.setPosition(x, y);
//}
//
//void Chicken::move_toward_player(float dt, MainObject* player)
//{
//    if (!player) return;
//
//    sf::FloatRect player_rect = player->get_rect();
//    float player_center_x = player_rect.left + player_rect.width / 2.0f;
//    float player_center_y = player_rect.top + player_rect.height / 2.0f;
//
//    float dx = player_center_x - x;
//    float dy = player_center_y - y;
//    float distance = std::sqrt(dx * dx + dy * dy);
//
//    if (distance > 0.1f)
//    {
//        x += (dx / distance) * speed_ * dt * 60.0f;
//        y += (dy / distance) * speed_ * dt * 60.0f;
//    }
//
//    anim_.sprite_.setPosition(x, y);
//}
//
//void Chicken::handle_shooting(float dt, MainObject* player)
//{
//    if (health_ <= 0 || !is_on_screen_) return;
//
//    sf::Time current_time = sf::Clock().getElapsedTime();
//    if (current_time - last_egg_time_ < sf::seconds(2.0f)) return;
//
//    int rand_num = rand() % 100;
//    if (rand_num <= egg_shooting_prob_)
//    {
//        std::unique_ptr<Egg> egg = std::make_unique<Egg>();
//        egg->set_position(x, y);
//
//        if (shooting_mode_ == ShootingMode::TOWARD_PLAYER && player)
//        {
//            sf::FloatRect player_rect = player->get_rect();
//            float target_x = player_rect.left + player_rect.width / 2.0f;
//            float target_y = player_rect.top + player_rect.height / 2.0f;
//
//            float dx = target_x - x;
//            float dy = target_y - y;
//            float distance = std::sqrt(dx * dx + dy * dy);
//
//            if (distance > 0.1f)
//            {
//                egg->set_velocity(dx / distance * 3.0f, dy / distance * 3.0f);
//            }
//        }
//        else
//        {
//            egg->set_velocity(0.0f, 2.0f); // Shoot downward
//        }
//
//        eggs_list_.push_back(std::move(egg));
//        play_laying_eggs_sound();
//        last_egg_time_ = current_time;
//    }
//}
//
//void Chicken::update_eggs(float dt)
//{
//    for (auto it = eggs_list_.begin(); it != eggs_list_.end();)
//    {
//        (*it)->update(dt);
//
//        if (!(*it)->is_on_screen() || (*it)->get_position().y > SCREEN_HEIGHT)
//        {
//            it = eggs_list_.erase(it);
//        }
//        else
//        {
//            ++it;
//        }
//    }
//}
//
//void Chicken::render_eggs(sf::RenderWindow& window)
//{
//    for (auto& egg : eggs_list_)
//    {
//        egg->render(window);
//    }
//}
//
//void Chicken::play_hit_sound()
//{
//    chicken_got_hit_sound_.play();
//}
//
//void Chicken::play_laying_eggs_sound()
//{
//    chicken_laying_eggs_sound_.play();
//}
//
//void Chicken::generate_present()
//{
//    if (has_present_ && present_)
//    {
//        present_->set_position(x, y);
//        present_->set_is_on_screen(true);
//    }
//}
//
//void Chicken::take_damage(int damage)
//{
//    health_ -= damage;
//    play_hit_sound();
//
//    if (health_ <= 0)
//    {
//        destroy();
//    }
//}
//
//void Chicken::destroy()
//{
//    is_on_screen_ = false;
//    generate_present();
//}