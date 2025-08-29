#include "AmmoObject.h"
#include <iostream>
#include <cmath>
#define M_PI 3.14159265358979323846

const int ARROW_WIDTH = 21;
const int ARROW_HEIGHT = 59;
const int BLASER_WIDTH = 30;
const int BLASER_HEIGHT = 60;
const int BORON_WIDTH = 25;
const int BORON_HEIGHT = 50;
const int NEURON_WIDTH = 28;
const int NEURON_HEIGHT = 55;

AmmoObject::AmmoObject()
    : Entity(), type_(AmmoType::ARROW), speed_(10), life_time_(1000), damage_(1),
    alive_(false), direction_(0), width_(ARROW_WIDTH), height_(ARROW_HEIGHT)
{
    x = 0;
    y = 0;
    radius_ = width_ / 2;
    if (!texture_.loadFromFile("res/image/arrow.png"))
        std::cout << "Failed to load texture: res/image/arrow.png" << std::endl;
    anim_ = Animation(texture_, 0, 0, width_, height_, 1, 0.0f); // Static sprite
    anim_.sprite_.setPosition(x, y);
    start_time_ = sf::Clock().getElapsedTime();
}

AmmoObject::~AmmoObject()
{
    free();
}

void AmmoObject::draw(sf::RenderWindow& window)
{
    if (alive_)
    {
        anim_.sprite_.setPosition(x, y);
        window.draw(anim_.sprite_);
    }
}

void AmmoObject::update()
{
    if (!alive_) return;
    sf::Time current_time = sf::Clock().getElapsedTime();
    if (current_time - start_time_ >= sf::milliseconds(life_time_))
    {
        alive_ = false;
        return;
    }
    x += speed_ * std::sin(direction_ * M_PI / 180);
    y -= speed_ * std::cos(direction_ * M_PI / 180);
    anim_.sprite_.setPosition(x, y);
    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT)
        alive_ = false;
}

bool AmmoObject::load_static_ammo_picture(const std::string& file_path)
{
    if (!texture_.loadFromFile(file_path))
    {
        std::cout << "Unable to load image " << file_path << std::endl;
        return false;
    }
    width_ = texture_.getSize().x;
    height_ = texture_.getSize().y;
    radius_ = width_ / 2;
    anim_ = Animation(texture_, 0, 0, width_, height_, 1, 0.0f);
    anim_.sprite_.setPosition(x, y);
    return true;
}

void AmmoObject::set_rect_cordinate(float x, float y)
{
    this->x = x;
    this->y = y;
    anim_.sprite_.setPosition(x, y);
}

void AmmoObject::set_rect_cordinate_width_height(float x, float y, int w, int h)
{
    this->x = x;
    this->y = y;
    width_ = w;
    height_ = h;
    radius_ = w / 2;
    anim_ = Animation(texture_, 0, 0, w, h, 1, 0.0f);
    anim_.sprite_.setPosition(x, y);
}

void AmmoObject::set_rect_width_height(float w, float h)
{
    width_ = w;
    height_ = h;
    radius_ = w / 2;
    anim_ = Animation(texture_, 0, 0, w, h, 1, 0.0f);
}

sf::FloatRect AmmoObject::get_rect() const
{
    return sf::FloatRect(x - width_ / 2, y - height_ / 2, width_, height_);
}

void AmmoObject::free()
{
    // SFML resources are cleaned up via RAII
}

ArrowAmmo::ArrowAmmo()
{
    type_ = AmmoType::ARROW;
    speed_ = 10;
    damage_ = 10;
    life_time_ = 1000;
    alive_ = true;
    width_ = ARROW_WIDTH;
    height_ = ARROW_HEIGHT;
    radius_ = width_ / 2;
    if (!texture_.loadFromFile("res/image/arrow.png"))
        std::cout << "Failed to load texture: res/image/arrow.png" << std::endl;
    anim_ = Animation(texture_, 0, 0, width_, height_, 1, 0.0f);
    anim_.sprite_.setPosition(x, y);
    start_time_ = sf::Clock().getElapsedTime();
}

ArrowAmmo::~ArrowAmmo()
{
    free();
}

BlaserAmmo::BlaserAmmo()
{
    type_ = AmmoType::BLASER;
    speed_ = 12;
    damage_ = 15;
    life_time_ = 800;
    alive_ = true;
    width_ = BLASER_WIDTH;
    height_ = BLASER_HEIGHT;
    radius_ = width_ / 2;
    if (!texture_.loadFromFile("res/image/NEUTRON0.png"))
        std::cout << "Failed to load texture: res/image/blaser.png" << std::endl;
    anim_ = Animation(texture_, 0, 0, width_, height_, 1, 0.0f);
    anim_.sprite_.setPosition(x, y);
    start_time_ = sf::Clock().getElapsedTime();
}

BlaserAmmo::~BlaserAmmo()
{
    free();
}

BoronAmmo::BoronAmmo()
{
    type_ = AmmoType::BORON;
    speed_ = 8;
    damage_ = 20;
    life_time_ = 1200;
    alive_ = true;
    width_ = BORON_WIDTH;
    height_ = BORON_HEIGHT;
    radius_ = width_ / 2;
    if (!texture_.loadFromFile("res/image/BORON0.png"))
        std::cout << "Failed to load texture: res/image/BORON0.png" << std::endl;
    anim_ = Animation(texture_, 0, 0, width_, height_, 1, 0.0f);
    anim_.sprite_.setPosition(x, y);
    start_time_ = sf::Clock().getElapsedTime();
}

BoronAmmo::~BoronAmmo()
{
    free();
}

NeuronAmmo::NeuronAmmo()
{
    type_ = AmmoType::NEURON;
    speed_ = 10;
    damage_ = 12;
    life_time_ = 1000;
    alive_ = true;
    width_ = NEURON_WIDTH;
    height_ = NEURON_HEIGHT;
    radius_ = width_ / 2;
    if (!texture_.loadFromFile("res/image/NEUTRON0.png"))
        std::cout << "Failed to load texture: res/image/neuron.png" << std::endl;
    anim_ = Animation(texture_, 0, 0, width_, height_, 1, 0.0f);
    anim_.sprite_.setPosition(x, y);
    start_time_ = sf::Clock().getElapsedTime();
}

NeuronAmmo::~NeuronAmmo()
{
    free();
}