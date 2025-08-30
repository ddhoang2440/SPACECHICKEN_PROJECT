//#include "Boss.h"
//#include <iostream>
//#include <cmath>
//#define M_PI 3.14159265358979323846
//static sf::Clock boss_clock;
//const sf::Time Boss::BOSS_spritetime = sf::milliseconds(100);
//int Boss::BOSS_spriteIndex = 0;
//
//Boss::Boss()
//    : Entity(), health_bar_width_(200), health_bar_height_(20), health_(100), x_direction_(1),
//    is_on_screen_(true), has_present_(true), speed_(5), width_of_sprite_(0), height_of_sprite_(0),
//    v_x_(0), v_y_(0), dir_x_(1), dir_y_(1), prev_collision_dir_("none")
//{
//    x = SCREEN_WIDTH / 2;
//    y = 100;
//    radius_ = 100; // Adjusted based on sprite size
//    health_bar_rect_ = sf::FloatRect(x - health_bar_width_ / 2, y - 10, health_bar_width_ * BOSS_SCALE, health_bar_height_ * BOSS_SCALE);
//    bgColor_ = sf::Color::Red;
//    fgColor_ = sf::Color::Green;
//    present_ = new Present(x,y);
//    present_->set_rect_cordinate(x, y);
//    last_egg_firing_time_ = boss_clock.getElapsedTime();
//    last_move_time_ = boss_clock.getElapsedTime();
//
//    if (!texture_.loadFromFile("res/image/boss.png"))
//        std::cout << "Failed to load texture: res/image/boss.png" << std::endl;
//    width_of_sprite_ = texture_.getSize().x / BOSS_NUMS_FRAME;
//    height_of_sprite_ = texture_.getSize().y;
//    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, BOSS_NUMS_FRAME, 0.2f);
//    anim_.sprite_.setPosition(x, y);
//
//    if (!shooting_egg_sound_buffer_.loadFromFile("res/sound/Laying_eggs.wav"))
//        std::cout << "Failed to load egg shot sound" << std::endl;
//    shooting_egg_sound_.setBuffer(shooting_egg_sound_buffer_);
//    if (!boss_hit_buffer_.loadFromFile("res/sound/Ci1bosshit.wav"))
//        std::cout << "Failed to load boss hit sound" << std::endl;
//    boss_hit_.setBuffer(boss_hit_buffer_);
//}
//
//Boss::~Boss()
//{
//    for (auto* egg : egg_list_)
//        delete egg;
//    egg_list_.clear();
//    delete present_;
//}
//
//void Boss::set_clips()
//{
//    if (width_of_sprite_ > 0 && height_of_sprite_ > 0)
//    {
//        anim_.frames_.clear();
//        for (int i = 0; i < BOSS_NUMS_FRAME; ++i)
//            anim_.frames_.push_back(sf::IntRect(i * width_of_sprite_, 0, width_of_sprite_, height_of_sprite_));
//        anim_.sprite_.setTextureRect(anim_.frames_[0]);
//    }
//}
//
//void Boss::load_animation_sprite(const std::string& file)
//{
//    if (!texture_.loadFromFile(file))
//    {
//        std::cout << "Failed to load texture: " << file << std::endl;
//        return;
//    }
//    width_of_sprite_ = texture_.getSize().x / BOSS_NUMS_FRAME;
//    height_of_sprite_ = texture_.getSize().y;
//    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, BOSS_NUMS_FRAME, 0.2f);
//    anim_.sprite_.setPosition(x, y);
//    radius_ = width_of_sprite_ * BOSS_SCALE / 2;
//}
//
//void Boss::render_health_bar(sf::RenderWindow& window)
//{
//    if (!is_on_screen_ || health_ <= 0) return;
//    float health_ratio = static_cast<float>(health_) / 100;
//    sf::RectangleShape bg(sf::Vector2f(health_bar_width_ * BOSS_SCALE, health_bar_height_ * BOSS_SCALE));
//    bg.setPosition(x - (health_bar_width_ * BOSS_SCALE) / 2, y - 10);
//    bg.setFillColor(bgColor_);
//    window.draw(bg);
//    sf::RectangleShape fg(sf::Vector2f(health_bar_width_ * BOSS_SCALE * health_ratio, health_bar_height_ * BOSS_SCALE));
//    fg.setPosition(x - (health_bar_width_ * BOSS_SCALE) / 2, y - 10);
//    fg.setFillColor(fgColor_);
//    window.draw(fg);
//}
//
//void Boss::draw(sf::RenderWindow& window)
//{
//    if (!is_on_screen_ || health_ <= 0) return;
//    sf::Time currentTicks = boss_clock.getElapsedTime();
//    if (currentTicks - last_move_time_ >= BOSS_spritetime)
//    {
//        BOSS_spriteIndex = (BOSS_spriteIndex + 1) % BOSS_NUMS_FRAME;
//        anim_.sprite_.setTextureRect(anim_.frames_[BOSS_spriteIndex]);
//        last_move_time_ = currentTicks;
//    }
//    anim_.sprite_.setPosition(x, y);
//    anim_.sprite_.setScale(BOSS_SCALE, BOSS_SCALE);
//    window.draw(anim_.sprite_);
//    render_health_bar(window);
//}
//
//void Boss::update(float dt)
//{
//    if (!is_on_screen_ || health_ <= 0) return;
//    anim_.update();
//    update_the_eggs();
//    if (has_present_)
//    {
//        present_->update(dt);
//        if (!present_->get_is_on_screen())
//            has_present_ = false;
//    }
//}
//
//void Boss::set_rect_cordinate(const float& x, const float& y)
//{
//    this->x = x;
//    this->y = y;
//    anim_.sprite_.setPosition(x, y);
//    health_bar_rect_.left = x - (health_bar_width_ * BOSS_SCALE) / 2;
//    health_bar_rect_.top = y - 10;
//}
//
//void Boss::set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height)
//{
//    this->x = x;
//    this->y = y;
//    width_of_sprite_ = width;
//    height_of_sprite_ = height;
//    anim_ = Animation(texture_, 0, 0, width, height, BOSS_NUMS_FRAME, 0.2f);
//    anim_.sprite_.setPosition(x, y);
//    health_bar_rect_.left = x - (health_bar_width_ * BOSS_SCALE) / 2;
//    health_bar_rect_.top = y - 10;
//    radius_ = width * BOSS_SCALE / 2;
//}
//
//void Boss::set_rect_width_and_height(const float& width, const float& height)
//{
//    width_of_sprite_ = width;
//    height_of_sprite_ = height;
//    anim_ = Animation(texture_, 0, 0, width, height, BOSS_NUMS_FRAME, 0.2f);
//    radius_ = width * BOSS_SCALE / 2;
//}
//
//sf::FloatRect Boss::get_rect() const
//{
//    return sf::FloatRect(x - (width_of_sprite_ * BOSS_SCALE) / 2, y - (height_of_sprite_ * BOSS_SCALE) / 2,
//        width_of_sprite_ * BOSS_SCALE, height_of_sprite_ * BOSS_SCALE);
//}
//
//sf::FloatRect Boss::get_rect_width_height_with_scale(const double& scale) const
//{
//    return sf::FloatRect(x - (width_of_sprite_ * scale) / 2, y - (height_of_sprite_ * scale) / 2,
//        width_of_sprite_ * scale, height_of_sprite_ * scale);
//}
//
//void Boss::firing_eggs()
//{
//    if (!is_on_screen_ || health_ <= 0) return;
//    sf::Time current_time = boss_clock.getElapsedTime();
//    if (current_time - last_egg_firing_time_ < sf::milliseconds(1000)) return;
//    if (egg_list_.size() >= BOSS_NUMBER_OF_EGGS) return;
//
//    double egg_speed = 5;
//    double angle_step = 2 * M_PI / 5;
//    for (int i = 0; i < 5; ++i)
//    {
//        double angle = i * angle_step + (static_cast<double>(std::rand()) / RAND_MAX) * angle_step;
//        double unit_x = std::cos(angle);
//        double unit_y = std::sin(angle);
//        Egg* egg = new Egg();
//        egg->set_rect_cordinate_width_and_height(x, y, EGG_WIDTH, EGG_HEIGHT);
//        egg->set_v_x(egg_speed * unit_x);
//        egg->set_v_y(egg_speed * unit_y);
//        egg_list_.push_back(egg);
//    }
//    last_egg_firing_time_ = current_time;
//    play_laying_eggs_sound();
//}
//
//void Boss::moving_toward_player(MainObject* main_object)
//{
//    if (!is_on_screen_ || health_ <= 0 || main_object->get_slow_move()) return;
//    sf::Time current_time = boss_clock.getElapsedTime();
//    if (current_time - last_move_time_ < MOVE_INTERVAL_MS) return;
//
//    double distance_x = main_object->x - x;
//    double distance_y = main_object->y - y;
//    double distance = std::sqrt(distance_x * distance_x + distance_y * distance_y);
//    if (distance == 0) return;
//    double unit_x = distance_x / distance;
//    double unit_y = distance_y / distance;
//    speed_ = 10;
//    v_x_ = speed_ * unit_x;
//    v_y_ = speed_ * unit_y;
//    x += v_x_;
//    y += v_y_;
//    anim_.sprite_.setPosition(x, y);
//    health_bar_rect_.left = x - (health_bar_width_ * BOSS_SCALE) / 2;
//    health_bar_rect_.top = y - 10;
//    last_move_time_ = current_time;
//}
//
//void Boss::update_the_eggs()
//{
//    for (auto it = egg_list_.begin(); it != egg_list_.end();)
//    {
//        (*it)->update();
//        if (!(*it)->get_alive())
//        {
//            delete* it;
//            it = egg_list_.erase(it);
//        }
//        else
//        {
//            ++it;
//        }
//    }
//}
//
//void Boss::render_the_eggs(sf::RenderWindow& window)
//{
//    for (auto* egg : egg_list_)
//        egg->render(window);
//}
//
//void Boss::play_hit_sound()
//{
//    boss_hit_.play();
//}
//
//void Boss::play_laying_eggs_sound()
//{
//    shooting_egg_sound_.play();
//}
//
//void Boss::generate_present()
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
#include "Boss.h"
#include <iostream>
#include <cmath>
#define M_PI 3.14159265358979323846
static sf::Clock boss_clock;
const sf::Time Boss::BOSS_spritetime = sf::milliseconds(100);
int Boss::BOSS_spriteIndex = 0;

Boss::Boss()
    : Entity(), health_bar_width_(200), health_bar_height_(20), health_(100), x_direction_(1),
    is_on_screen_(true), has_present_(true), speed_(5), width_of_sprite_(0), height_of_sprite_(0),
    v_x_(0), v_y_(0), dir_x_(1), dir_y_(1), prev_collision_dir_("none"), targetX_(0),
    targetY_(0),
    hasTarget_(false),
    reachedTarget_(false),
    entranceSpeed_(4.0f),
    isEntering_(true)
{
    x = SCREEN_WIDTH / 2;
    y = 100;
    radius_ = 100; // Adjusted based on sprite size
    health_bar_rect_ = sf::FloatRect(x - health_bar_width_ / 2, y - 10, health_bar_width_ * BOSS_SCALE, health_bar_height_ * BOSS_SCALE);
    bgColor_ = sf::Color::Red;
    fgColor_ = sf::Color::Green;
    present_ = new Present(x,y);
	last_egg_firing_time_ = sf::Time::Zero;
    last_move_time_ = boss_clock.getElapsedTime();

    if (!texture_.loadFromFile("res/image/boss.png"))
        std::cout << "Failed to load texture: res/image/boss.png" << std::endl;
    width_of_sprite_ = texture_.getSize().x / BOSS_NUMS_FRAME;
    height_of_sprite_ = texture_.getSize().y;
    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, BOSS_NUMS_FRAME, 0.2f);
    anim_.sprite_.setPosition(x, y);

    if (!shooting_egg_sound_buffer_.loadFromFile("res/sound/Laying_eggs.wav"))
        std::cout << "Failed to load egg shot sound" << std::endl;
    shooting_egg_sound_.setBuffer(shooting_egg_sound_buffer_);
    if (!boss_hit_buffer_.loadFromFile("res/sound/Ci1bosshit.wav"))
        std::cout << "Failed to load boss hit sound" << std::endl;
    boss_hit_.setBuffer(boss_hit_buffer_);
}

Boss::~Boss()
{
    for (auto* egg : egg_list_)
        delete egg;
    egg_list_.clear();
    delete present_;
}

void Boss::set_clips()
{
    if (width_of_sprite_ > 0 && height_of_sprite_ > 0)
    {
        anim_.frames_.clear();
        for (int i = 0; i < BOSS_NUMS_FRAME; ++i)
            anim_.frames_.push_back(sf::IntRect(i * width_of_sprite_, 0, width_of_sprite_, height_of_sprite_));
        anim_.sprite_.setTextureRect(anim_.frames_[0]);
    }
}

void Boss::load_animation_sprite(const std::string& file)
{
    if (!texture_.loadFromFile(file))
    {
        std::cout << "Failed to load texture: " << file << std::endl;
        return;
    }
    width_of_sprite_ = texture_.getSize().x / BOSS_NUMS_FRAME;
    height_of_sprite_ = texture_.getSize().y;
    anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, BOSS_NUMS_FRAME, 0.2f);
    anim_.sprite_.setPosition(x, y);
    radius_ = width_of_sprite_ * BOSS_SCALE / 2;
}

void Boss::render_health_bar(sf::RenderWindow& window)
{
    if (!is_on_screen_ || health_ <= 0) return;
    float health_ratio = static_cast<float>(health_) / 100;
    sf::RectangleShape bg(sf::Vector2f(health_bar_width_ * BOSS_SCALE, health_bar_height_ * BOSS_SCALE));
    bg.setPosition(x - (health_bar_width_ * BOSS_SCALE) / 2, y - 10);
    bg.setFillColor(bgColor_);
    window.draw(bg);
    sf::RectangleShape fg(sf::Vector2f(health_bar_width_ * BOSS_SCALE * health_ratio, health_bar_height_ * BOSS_SCALE));
    fg.setPosition(x - (health_bar_width_ * BOSS_SCALE) / 2, y - 10);
    fg.setFillColor(fgColor_);
    window.draw(fg);
}

void Boss::draw(sf::RenderWindow& window)
{
    if (!is_on_screen_ || health_ <= 0) return;
    sf::Time currentTicks = boss_clock.getElapsedTime();
    if (currentTicks - last_move_time_ >= BOSS_spritetime)
    {
        BOSS_spriteIndex = (BOSS_spriteIndex + 1) % BOSS_NUMS_FRAME;
        anim_.sprite_.setTextureRect(anim_.frames_[BOSS_spriteIndex]);
        last_move_time_ = currentTicks;
    }
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setScale(BOSS_SCALE, BOSS_SCALE);
    window.draw(anim_.sprite_);
    render_health_bar(window);
}

void Boss::update(float dt)
{
    if (!is_on_screen_ || health_ <= 0) return;
    anim_.update();
    if (isEntering_ && hasTarget_) {
        performEntrance();
    }
    if (!isEntering_) {
        update_the_eggs(dt);
    }
    if (has_present_)
    {
        present_->update(dt);
        if (!present_->get_is_on_screen())
            has_present_ = false;
    }
}

void Boss::set_rect_cordinate(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
    anim_.sprite_.setPosition(x, y);
    health_bar_rect_.left = x - (health_bar_width_ * BOSS_SCALE) / 2;
    health_bar_rect_.top = y - 10;
}

void Boss::set_rect_cordinate_width_and_height(const float& x, const float& y, const float& width, const float& height)
{
    this->x = x;
    this->y = y;
    width_of_sprite_ = width;
    height_of_sprite_ = height;
    anim_ = Animation(texture_, 0, 0, width, height, BOSS_NUMS_FRAME, 0.2f);
    anim_.sprite_.setPosition(x, y);
    health_bar_rect_.left = x - (health_bar_width_ * BOSS_SCALE) / 2;
    health_bar_rect_.top = y - 10;
    radius_ = width * BOSS_SCALE / 2;
}

void Boss::set_rect_width_and_height(const float& width, const float& height)
{
    width_of_sprite_ = width;
    height_of_sprite_ = height;
    anim_ = Animation(texture_, 0, 0, width, height, BOSS_NUMS_FRAME, 0.2f);
    radius_ = width * BOSS_SCALE / 2;
}

sf::FloatRect Boss::get_rect() const
{
    return sf::FloatRect(x - (width_of_sprite_ * BOSS_SCALE) / 2, y - (height_of_sprite_ * BOSS_SCALE) / 2,
        width_of_sprite_ * BOSS_SCALE, height_of_sprite_ * BOSS_SCALE);
}

sf::FloatRect Boss::get_rect_width_height_with_scale(const double& scale) const
{
    return sf::FloatRect(x - (width_of_sprite_ * scale) / 2, y - (height_of_sprite_ * scale) / 2,
        width_of_sprite_ * scale, height_of_sprite_ * scale);
}

//void Boss::firing_eggs()
//{
//    if (!is_on_screen_ || health_ <= 0) return;
//    if (isEntering_)
//        return;
//    sf::Time current_time = boss_clock.getElapsedTime();
//    if (current_time - last_egg_firing_time_ < sf::milliseconds(1000)) return;
//    if (egg_list_.size() >= BOSS_NUMBER_OF_EGGS) return;
//
//    double egg_speed = 5;
//    double angle_step = 2 * M_PI / 5;
//    for (int i = 0; i < 5; ++i)
//    {
//        double angle = i * angle_step + (static_cast<double>(std::rand()) / RAND_MAX) * angle_step;
//        double unit_x = std::cos(angle);
//        double unit_y = std::sin(angle);
//        Egg* egg = new Egg();
//        egg->set_rect_cordinate_width_and_height(x, y, EGG_WIDTH, EGG_HEIGHT);
//        egg->set_v_x(egg_speed * unit_x);
//        egg->set_v_y(egg_speed * unit_y);
//        egg_list_.push_back(egg);
//    }
//    last_egg_firing_time_ = current_time;
//    play_laying_eggs_sound();
//}
void Boss::firing_eggs() {
    if (!is_on_screen_ || health_ <= 0 || isEntering_) {
        std::cout << "Cannot fire eggs: on_screen=" << is_on_screen_
            << ", health=" << health_ << ", isEntering=" << isEntering_ << "\n";
        return;
    }
    sf::Time current_time = boss_clock.getElapsedTime();
    std::cout << "Time since last egg: " << (current_time - last_egg_firing_time_).asSeconds() << "s\n";
    if (current_time - last_egg_firing_time_ < sf::milliseconds(1000)) {
        return;
    }
    if (egg_list_.size() >= BOSS_NUMBER_OF_EGGS) {
        return;
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    double egg_speed = 2.0;
    double angle_step = 2.0 * M_PI / 5.0;
    for (int i = 0; i < 5; ++i) {
        double angle = i * angle_step + dist(gen) * angle_step;
        double unit_x = std::cos(angle);
        double unit_y = std::sin(angle);
        auto egg = std::make_unique<Egg>();
        egg->set_rect_cordinate_width_and_height(x, y, EGG_WIDTH, EGG_HEIGHT);
        egg->set_v_x(egg_speed * unit_x);
        egg->set_v_y(egg_speed * unit_y);
        egg_list_.push_back(egg.release());
    }
    last_egg_firing_time_ = current_time;
    play_laying_eggs_sound();
}
void Boss::moving_toward_player(MainObject* main_object)
{
    if (!is_on_screen_ || health_ <= 0 || main_object->get_slow_move()) return;
    if (isEntering_)
        return;
    sf::Time current_time = boss_clock.getElapsedTime();
    if (current_time - last_move_time_ < MOVE_INTERVAL_MS) return;

    double distance_x = main_object->x - x;
    double distance_y = main_object->y - y;
    double distance = std::sqrt(distance_x * distance_x + distance_y * distance_y);
    if (distance == 0) return;
    double unit_x = distance_x / distance;
    double unit_y = distance_y / distance;
    speed_ = 10;
    v_x_ = speed_ * unit_x;
    v_y_ = speed_ * unit_y;
    x += v_x_;
    y += v_y_;
    anim_.sprite_.setPosition(x, y);
    health_bar_rect_.left = x - (health_bar_width_ * BOSS_SCALE) / 2;
    health_bar_rect_.top = y - 10;
    last_move_time_ = current_time;
}

void Boss::update_the_eggs(float dt)
{
    for (auto it = egg_list_.begin(); it != egg_list_.end();)
    {
        Egg* egg = *it;

        float new_x = egg->get_x() + egg->get_v_x() * dt * 60.0f;
        float new_y = egg->get_y() + egg->get_v_y() * dt * 60.0f;
        egg->set_rect_cordinate(new_x, new_y);

        if (new_y > SCREEN_HEIGHT || !egg->get_alive())
        {
            delete egg;
            it = egg_list_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Boss::render_the_eggs(sf::RenderWindow& window)
{
    for (auto* egg : egg_list_)
        egg->render(window);
}

void Boss::play_hit_sound()
{
    boss_hit_.play();
}

void Boss::play_laying_eggs_sound()
{
    shooting_egg_sound_.play();
}

void Boss::generate_present()
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
void Boss::setTargetPosition(float x, float y) {
    targetX_ = x;
    targetY_ = y;
    hasTarget_ = true;
    reachedTarget_ = false;
    isEntering_ = true;
}

void Boss::performEntrance() {
    if (!hasTarget_ || reachedTarget_ || !is_on_screen_ || health_ <= 0)
        return;

    float dx = targetX_ - x;
    float dy = targetY_ - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 5.0f) {
        x = targetX_;
        y = targetY_;
        reachedTarget_ = true;
        isEntering_ = false;
       
        std::cout << "Boss has reached battle position!" << std::endl;
    }
    else {

        float moveX = (dx / distance) * entranceSpeed_;
        float moveY = (dy / distance) * entranceSpeed_;

        x += moveX;
        y += moveY;
    }

    anim_.sprite_.setPosition(x, y);
    health_bar_rect_.left = x - (health_bar_width_ * BOSS_SCALE) / 2;
    health_bar_rect_.top = y - 10;
}