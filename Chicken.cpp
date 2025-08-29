#include "Chicken.h"
#include <cmath>
#include <iostream>


const sf::Time Chicken::CHICKEN_spritetime = sf::milliseconds(50);
int Chicken::CHICKEN_spriteIndex = 0;
const int CHICKEN_NUMS_FRAME = 4;
Chicken::Chicken()
    : Entity(), health_(2), speed_(10), egg_shooting_prob_(40), scale_(CHICKEN_OBJECT_SCALE),
    angle_(90), angle_increment_(2), is_on_screen_(true), has_wing_(false),
    has_present_(false), width_of_sprite_(0), height_of_sprite_(0),
    v_x_(4.0), v_y_(4.0), dir_x_(1), dir_y_(1), x_direction_(1), y_direction_(1.0f),
    wing_fall_speed_(10), damage_(0), type_(1), direction_(0.0),
    movement_pattern_(MovementPattern::UP_DOWN), shoot_interval_(1.0f), shoot_timer_(0.0f),
    rng_(std::random_device{}()), dist_100_(0, 100), dist_6_(0, 5), center_x_(SCREEN_WIDTH / 2.f), 
    center_y_(SCREEN_HEIGHT / 2.f),  
    min_y_(50.f),                     
	max_y_(SCREEN_HEIGHT - 50.f), shooting_mode_(ShootingMode::DOWNWARD)
{
    x = 0;
    y = 0;
    radius_ = 75 / 2.0f; // Based on rect_.width = 75
    wing_rect_ = sf::FloatRect(0, 0, 59, 84);
    last_egg_time_ = sf::Time::Zero;
    broken_egg_time_ = sf::Time::Zero;
    last_wing_time_ = sf::Time::Zero;
    last_move_time_ = sf::Time::Zero;
    // Khởi tạo has_wing_ và has_present_ với xác suất
    has_wing_ = dist_100_(rng_) <= 70;
    has_present_ = dist_100_(rng_) <= 50;

    // Load textures
    if (!texture_.loadFromFile("res/image/chicken123.png")) {
        std::cerr << "Failed to load chicken texture: res/image/chicken123.png" << std::endl;
    }

    if (!wing_texture_.loadFromFile("res/image/meat.png")) {
        std::cerr << "Failed to load wing texture: res/image/meat.png" << std::endl;
    }
    width_of_sprite_ = texture_.getSize().x / CHICKEN_NUMS_FRAME;
    height_of_sprite_ = texture_.getSize().y;
    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, CHICKEN_NUMS_FRAME, 0.05f);
    anim_.sprite_.setPosition(x, y);
    // Load sounds
    if (!chicken_got_hit_buffer_.loadFromFile("res/sound/CHICKEN_GOT_HIT.wav")) {
        std::cerr << "Failed to load chicken hit sound: res/sound/CHICKEN_GOT_HIT.wav" << std::endl;
    }

    if (!chicken_laying_eggs_buffer_.loadFromFile("res/sound/Laying_eggs.wav")) {
        std::cerr << "Failed to load laying eggs sound: res/sound/Laying_eggs.wav" << std::endl;
    }

    if (!eggs_get_destroyed_buffer_.loadFromFile("res/sound/Egg_Destroy.wav")) {
        std::cerr << "Failed to load egg destroyed sound: res/sound/Egg_Destroy.wav" << std::endl;
    }
    chicken_got_hit_sound_.setBuffer(chicken_got_hit_buffer_);
    chicken_laying_eggs_sound_.setBuffer(chicken_laying_eggs_buffer_);
    eggs_get_destroyed_sound_.setBuffer(eggs_get_destroyed_buffer_);
    // Khởi tạo present_ nếu cần, sử dụng rand()
    if (dist_100_(rng_) <= 50) { // 50% chance để có present
        has_present_ = true;
        BonusType kind = static_cast<BonusType>(dist_6_(rng_));
        present_ = std::make_unique<Present>(kind, x, y);
    }
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
    anim_.sprite_.setOrigin(width_of_sprite_ / 2.f, height_of_sprite_ / 2.f);
}

void Chicken::set_rect_cordinate(const float& x, const float& y)
{
    this->x = x;
    this->y = y;

//     anim_.sprite_.setPosition(x, y);
//     wing_rect_.left = x - width_of_sprite_ / 2.f;
//     wing_rect_.top = y - height_of_sprite_ / 2.f;
//     wing_rect_.width = width_of_sprite_;

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

    // Cập nhật animation gà
    anim_.update();

    // Cập nhật trứng
    update_eggs(dt);

    // Cập nhật present nếu còn tồn tạis
    if (has_present_ && present_) {
        present_->update(dt);

        // Nếu present đã rời màn hình, reset unique_ptr
        if (!present_->get_is_on_screen()) {
            has_present_ = false;
            present_.reset(); // tránh dangling pointer
        }
    }
}

void Chicken::update_eggs(float dt)
{
    for (auto it = eggs_list_.begin(); it != eggs_list_.end();) {
        if (!(*it)->get_alive()) {
            eggs_get_destroyed_sound_.play();
            it = eggs_list_.erase(it);
        }
        else {
            float new_x = (*it)->get_x() + (*it)->get_v_x() * dt;
            float new_y = (*it)->get_y() + (*it)->get_v_y() * dt;
            (*it)->set_rect_cordinate(new_x, new_y);
            if (new_y > SCREEN_HEIGHT) {
                eggs_get_destroyed_sound_.play();
                it = eggs_list_.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void Chicken::handle_shooting_eggs_downward(float dt)
{
    if (health_ <= 0 || !is_on_screen_) return;

    shoot_timer_ += dt;
    if (shoot_timer_ >= shoot_interval_) {
        if (dist_100_(rng_) < 50) {
            auto egg = std::make_unique<Egg>();
            if (!egg) {
                std::cerr << "Failed to create Egg object" << std::endl;
                return;
            }
            float egg_x = x;
            float egg_y = y + (height_of_sprite_ * scale_) + 5.0f;
            egg->set_rect_cordinate(egg_x, egg_y);
            egg->set_v_x(0.0f);
            egg->set_v_y(4.0f + (dist_100_(rng_) % 3));
            egg->set_alive(true);
            eggs_list_.push_back(std::move(egg));
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


    if (dist_100_(rng_) <= egg_shooting_prob_) {
        double dx = main_object->get_rect().left + main_object->get_rect().width / 2 - x;
        double dy = main_object->get_rect().top + main_object->get_rect().height / 2 - y;
        double distance = std::sqrt(dx * dx + dy * dy);
        if (distance > 0) {
            double unit_x = dx / distance;
            double unit_y = dy / distance;
            double egg_speed = 9.0;
            auto egg = std::make_unique<Egg>();
            if (!egg) {
                std::cerr << "Failed to create Egg object" << std::endl;
                return;
            }

            egg->set_rect_cordinate_width_and_height(x, y, EGG_WIDTH, EGG_HEIGHT);
            egg->set_v_x(egg_speed * unit_x);
            egg->set_v_y(egg_speed * unit_y);
            egg->set_alive(true);
            eggs_list_.push_back(std::move(egg));
            chicken_laying_eggs_sound_.play();
        }
        shoot_timer_ = 0.f;

    }
}

void Chicken::update_the_eggs()
{
    for (auto it = eggs_list_.begin(); it != eggs_list_.end();) {
        if (!(*it)->get_alive()) {
            eggs_get_destroyed_sound_.play();
            it = eggs_list_.erase(it);
        }
        else {
            (*it)->update();
            ++it;
        }
    }
}


void Chicken::render_the_eggs(sf::RenderWindow& window)
{
    for (const auto& egg : eggs_list_) {
        egg->render(window);
    }
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


std::unique_ptr<Present> Chicken::generate_present()
{
    if (!has_present_) return nullptr;

    BonusType new_type = static_cast<BonusType>(dist_6_(rng_));
    auto p = std::make_unique<Present>(new_type, x, y);
    p->set_is_on_screen(true);

    // Reset trạng thái present trong Chicken
    has_present_ = false;
    present_.reset();  // Xóa present cũ

    return p;          // Trả về unique_ptr mới
}

void Chicken::moving_diagonally()
{
    if (!is_on_screen_ || health_ <= 0) return;
    x += v_x_ * dir_x_;
    y += v_y_ * dir_y_;
    if (x < 0) dir_x_ = 1;
    else if (x + width_of_sprite_ > SCREEN_WIDTH) dir_x_ = -1;
    if (y < 0) dir_y_ = 1;
    else if (y + height_of_sprite_ > SCREEN_HEIGHT) dir_y_ = -1;
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
}

void Chicken::moving_parabola()
{
    if (!is_on_screen_ || health_ <= 0) return;
    double a = -0.005;
    double h = SCREEN_WIDTH / 2.0;
    double k = SCREEN_HEIGHT / 2.0;
    x += x_direction_ * speed_;
    y = a * std::pow(x - h, 2) + k;

    if (x <= 0 || x + width_of_sprite_ >= SCREEN_WIDTH) x_direction_ *= -1;
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
}

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
    if (x <= 0 || x + width_of_sprite_ >= SCREEN_WIDTH) x_direction_ *= -1;
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
}

void Chicken::moving_like_a_circle()
{
    if (!is_on_screen_ || health_ <= 0) return;

    // Update góc quay
    angle_ += angle_increment_;
    if (angle_ >= 360.f) angle_ -= 360.f;

    // Nhấp nhô radius
    radius_ += angle_speed_; // dùng angle_speed_ làm delta radius
    if (radius_ >= 150.f || radius_ <= 50.f) // radius max/min có thể điều chỉnh
        angle_speed_ = -angle_speed_;

    // Tính vị trí x, y theo góc và radius hiện tại
    x = center_x_ + radius_ * std::cos(angle_ * M_PI / 180.f);
    y = center_y_ + radius_ * std::sin(angle_ * M_PI / 180.f);

    // Cập nhật sprite và wing_rect
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

    if (distance > 0) {

        x += (dx / distance) * speed_;
        y += (dy / distance) * speed_;
        anim_.sprite_.setPosition(x, y);
        wing_rect_.left = x - wing_rect_.width / 2;
        wing_rect_.top = y - wing_rect_.height / 2;
    }
}

void Chicken::free()
{
    if (!eggs_list_.empty()) {
        std::cout << "Clearing " << eggs_list_.size() << " eggs from Chicken at (" << x << ", " << y << ")" << std::endl;
        eggs_list_.clear();
    }
    if (present_) {
        std::cout << "Clearing present from Chicken at (" << x << ", " << y << ")" << std::endl;
        present_.reset();
    }
    has_present_ = false;
    has_wing_ = false;
    texture_ = sf::Texture();
    wing_texture_ = sf::Texture();
    anim_ = Animation();
}

void Chicken::moving_down()
{
    if (!is_on_screen_ || health_ <= 0) return;
    y += speed_;
    if (y > SCREEN_HEIGHT) is_on_screen_ = false;
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
}
