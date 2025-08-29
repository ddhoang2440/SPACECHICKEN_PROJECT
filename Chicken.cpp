#include "Chicken.h"
#include <cmath>
#include <iostream>
#define M_PI 3.14159265358979323846

const sf::Time Chicken::CHICKEN_spritetime = sf::milliseconds(50);
int Chicken::CHICKEN_spriteIndex = 0;

Chicken::Chicken()
    : Entity(), health_(2), speed_(10), egg_shooting_prob_(50), scale_(CHICKEN_OBJECT_SCALE),
    angle_(90), angle_increment_(2), is_on_screen_(true), has_wing_(rand() % 100 <= 70),
    has_present_(rand() % 100 <= 50), width_of_sprite_(0), height_of_sprite_(0),
    v_x_(4.0), v_y_(4.0), dir_x_(1), dir_y_(1), x_direction_(1), wing_fall_speed_(10),
    damage_(0), type_(1), direction_(0.0),targetX_(0),
    targetY_(0),
    hasTarget_(false),
    reachedTarget_(false),
    formationSpeed_(3.0f)
{
    x = 0;
    y = 0;
    radius_ = 75 / 2; // Based on rect_.width = 75
    present_ = new Present(static_cast<BonusType>(rand() % 6));
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
}

void Chicken::set_rect_cordinate(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - 59 / 2;
    wing_rect_.top = y - 84 / 2;
}

void Chicken::set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height)
{
    this->x = x;
    this->y = y;
    width_of_sprite_ = width;
    height_of_sprite_ = height;
    anim_ = Animation(texture_, 0, 0, width, height, CHICKEN_NUMS_FRAME, 0.05f);
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - 59 / 2;
    wing_rect_.top = y - 84 / 2;
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

void Chicken::update()
{
    if (!is_on_screen_ || health_ <= 0) return;
    anim_.update();
    // Ưu tiên di chuyển đến formation trước
    if (hasTarget_ && !reachedTarget_) {
        moveToFormation();
    }else {
        moving_diagonally(); // Default movement, can be changed to other patterns
    }
    
    for (auto* egg : eggs_list_)
        egg->update();
    if (has_present_)
    {
        present_->update();
        if (!present_->get_is_on_screen())
            has_present_ = false;
    }
}

void Chicken::handle_shooting_eggs_toward_player(MainObject* main_object)
{
    if (main_object->get_slow_move() || health_ <= 0 || !is_on_screen_) return;
    sf::Time current_time = sf::Clock().getElapsedTime();
    if (current_time - last_egg_time_ < sf::seconds(5)) return;

    int rand_num = rand() % 100 + 1;
    if (rand_num <= egg_shooting_prob_)
    {
        double dx = main_object->get_rect().left + main_object->get_rect().width / 2 - x;
        double dy = main_object->get_rect().top + main_object->get_rect().height / 2 - y;
        double distance = std::sqrt(dx * dx + dy * dy);
        double unit_x = dx / distance;
        double unit_y = dy / distance;
        double egg_speed = 9;
        Egg* egg = new Egg();
        egg->set_rect_cordinate_width_and_height(x, y, EGG_WIDTH, EGG_HEIGHT);
        egg->set_v_x(egg_speed * unit_x);
        egg->set_v_y(egg_speed * unit_y);
        eggs_list_.push_back(egg);
        chicken_laying_eggs_sound_.play();
        last_egg_time_ = current_time;
    }
}

void Chicken::shooting_eggs_toward_player()
{
    handle_shooting_eggs_toward_player(nullptr); // Placeholder, requires MainObject
}

void Chicken::update_the_eggs()
{
    for (auto it = eggs_list_.begin(); it != eggs_list_.end();)
    {
        (*it)->update();
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

void Chicken::generate_present()
{
    if (!has_present_) return;
    if (health_ > 0)
    {
        present_->set_kind_of_present(static_cast<BonusType>(rand() % 6));
        if (present_->get_kind_of_present() == ARROW)
            present_->set_kind_of_present(BORON); // Avoid ARROW
        present_->set_rect_cordinate(x, y);
    }
    else
    {
        present_->set_is_on_screen(true);
    }
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

void Chicken::moving_like_a_circle()
{
    if (!is_on_screen_ || health_ <= 0) return;
    angle_ += angle_increment_;
    if (angle_ >= 360) angle_ -= 360;
    x = SCREEN_WIDTH / 2 + radius_ * std::cos(angle_ * M_PI / 180);
    y = SCREEN_HEIGHT / 2 + radius_ * std::sin(angle_ * M_PI / 180);
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
    if (present_)
    {
        delete present_;
        present_ = nullptr;
    }
}
//setTargetPosition(x, y) = giao “nhiệm vụ” cho gà : “mày phải bay tới chỗ này trong đội hình”.
//moveToFormation() = mỗi frame update, gà sẽ tự động bay dần đến chỗ đó.
void Chicken::setTargetPosition(float x, float y) {
    targetX_ = x;
    targetY_ = y;
    hasTarget_ = true;
    reachedTarget_ = false;
}
void Chicken::moveToFormation() {
    if (!hasTarget_ || reachedTarget_ || !is_on_screen_ || health_ <= 0)
        return;

    // Tính khoảng cách đến vị trí mục tiêu
    float dx = targetX_ - x;
    float dy = targetY_ - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Nếu đã đến gần vị trí mục tiêu
    if (distance < 5.0f) {
        x = targetX_;
        y = targetY_;
        reachedTarget_ = true;
        hasTarget_ = false;
    }
    else {
        // Di chuyển dần về vị trí mục tiêu
        float moveX = (dx / distance) * formationSpeed_;
        float moveY = (dy / distance) * formationSpeed_;

        x += moveX;
        y += moveY;
    }

    // Cập nhật vị trí sprite
    anim_.sprite_.setPosition(x, y);
    wing_rect_.left = x - wing_rect_.width / 2;
    wing_rect_.top = y - wing_rect_.height / 2;
}