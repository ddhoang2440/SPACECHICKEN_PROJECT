#ifndef AMMOOBJECT_H_
#define AMMOOBJECT_H_
#include "CommonVariable.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
enum class AmmoType
{
    ARROW = 1,
    BLASER = 2,
    BORON = 3,
    NEURON = 4
};
class AmmoObject : public Entity
{
protected:
    sf::Texture texture_;
    int life_time_; // Lifetime in milliseconds
    float direction_; // Direction in degrees
    int speed_; // Speed of the ammo
    int damage_; // Damage dealt by the ammo
    bool alive_; // Whether ammo is active
    int width_; // Sprite width
    int height_; // Sprite height
    sf::Time start_time_; // Time when ammo is created

public:
    
    AmmoType type_;

    AmmoObject();
    virtual ~AmmoObject();

    void set_type(AmmoType type) { type_ = type; }
    AmmoType get_type() const { return type_; }

	virtual void draw(sf::RenderWindow& window) override;
    virtual void update() override;

    bool load_static_ammo_picture(const std::string& file_path);

    int get_speed() const { return speed_; }
    void set_speed(int speed) { speed_ = speed; }

    sf::FloatRect get_rect() const override;
    void set_rect_cordinate(float x, float y);
    void set_rect_cordinate_width_height(float x, float y, int w, int h);
    void set_rect_width_height(float w, float h);
	void set_rotation(float angle) { direction_ = angle; anim_.sprite_.setRotation(angle); }
	float get_rotation() const { return anim_.sprite_.getRotation(); }
	sf::Sprite get_sprite() const { return anim_.sprite_; }

    bool is_alive() const { return alive_; }
    void set_alive(bool alive) { alive_ = alive; }

    int get_damage() const { return damage_; }
    void set_damage(int damage) { damage_ = damage; }

    void set_direction(float direction) { direction_ = direction; }
    double get_direction() const { return direction_; }

    const sf::Texture& get_texture() const { return texture_; }

    void free();
};

class ArrowAmmo : public AmmoObject
{
public:
    ArrowAmmo();
    ~ArrowAmmo();
};

class BlaserAmmo : public AmmoObject
{
public:
    BlaserAmmo();
    ~BlaserAmmo();
};

class BoronAmmo : public AmmoObject
{
public:
    BoronAmmo();
    ~BoronAmmo();
};

class NeuronAmmo : public AmmoObject
{
public:
    NeuronAmmo();
    ~NeuronAmmo();
};

#endif