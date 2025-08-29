#include "Present.h"
#include <random>
#include <iostream>

Present::Present(float x, float y)
    : Entity(), speed_(15.0f), is_on_screen_(false)
{
    // Randomly select BonusType
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 5);
    kind_of_present_ = static_cast<BonusType>(dist(rng));

    // Set position and radius
    this->x = x;
    this->y = y;
    radius_ = PRESENT_DEFAULT_WIDTH / 2.0f;

    // Load texture
    if (!load_texture()) {
        std::cerr << "Error: Failed to load texture for present type " << kind_of_present_ << std::endl;
        // Fallback to a default texture or handle error as needed
        return;
    }

    // Initialize animation and sprite
    float width = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_WIDTH : PRESENT_DEFAULT_WIDTH;
    float height = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_HEIGHT : PRESENT_DEFAULT_HEIGHT;
    anim_ = Animation(texture_, 0, 0, width, height, 1, 0);
    anim_.sprite_.setTexture(texture_);
    anim_.sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setScale(width / texture_.getSize().x, height / texture_.getSize().y);
}

Present::Present(BonusType kind, float x, float y)
    : Entity(), speed_(15.0f), kind_of_present_(kind), is_on_screen_(true)
{
    // Set position and radius
    this->x = x;
    this->y = y;
    radius_ = PRESENT_DEFAULT_WIDTH / 2.0f;

    // Load texture
    if (!load_texture()) {
        std::cerr << "Error: Failed to load texture for present type " << kind_of_present_ << std::endl;
        // Fallback to a default texture or handle error as needed
        return;
    }

    // Initialize animation and sprite
    float width = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_WIDTH : PRESENT_DEFAULT_WIDTH;
    float height = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_HEIGHT : PRESENT_DEFAULT_HEIGHT;
    anim_ = Animation(texture_, 0, 0, width, height, 1, 0);
    anim_.sprite_.setTexture(texture_);
    anim_.sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
    anim_.sprite_.setPosition(x, y);
    anim_.sprite_.setScale(width / texture_.getSize().x, height / texture_.getSize().y);
}

Present::~Present()
{
}

bool Present::load_texture()
{
    bool loaded = false;
    switch (kind_of_present_)
    {
    case ATOMIC_POWER:
        loaded = texture_.loadFromFile("res/image/power_up.png");
        break;
    case NEUTRON:
        loaded = texture_.loadFromFile("res/image/neutron_gift.png");
        break;
    case ARROW:
        loaded = texture_.loadFromFile("res/image/arrow_gift.png");
        break;
    case BORON:
        loaded = texture_.loadFromFile("res/image/boron_gift.png");
        break;
    case SHIELD:
        loaded = texture_.loadFromFile("res/image/shield.png");
        break;
    case LIFE:
        loaded = texture_.loadFromFile("res/image/heart.png");
        break;
    default:
        std::cerr << "Error: Unknown present type " << kind_of_present_ << std::endl;
        break;
    }
    return loaded;
}

void Present::set_rect_cordinate(float x, float y)
{
    this->x = x;
    this->y = y;
    anim_.sprite_.setPosition(x, y);
}

sf::FloatRect Present::get_rect() const
{
    float width = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_WIDTH : PRESENT_DEFAULT_WIDTH;
    float height = (kind_of_present_ == ATOMIC_POWER) ? PRESENT_ATOMIC_HEIGHT : PRESENT_DEFAULT_HEIGHT;
    return sf::FloatRect(x - width / 2.0f, y - height / 2.0f, width, height);
}

void Present::update(float deltaTime)
{
    if (!is_on_screen_) return;

    // Move downward
    y += speed_*10 * deltaTime;
    anim_.sprite_.setPosition(x, y);

    // Check if off-screen
    if (y > SCREEN_HEIGHT)
    {
        is_on_screen_ = false;
    }

    // Update animation (if multi-frame animation is used)
    anim_.update();
}

void Present::render(sf::RenderWindow& window)
{
    if (is_on_screen_)
    {
        window.draw(anim_.sprite_);
    }
}

BonusType Present::get_kind() const
{
    return kind_of_present_;
}