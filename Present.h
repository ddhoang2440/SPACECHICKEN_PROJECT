#ifndef PRESENT_H
#define PRESENT_H
#include "CommonVariable.h"
#include "Entity.h"
#include "Animation.h"

class Present : public Entity
{
protected:
    sf::Texture texture_;
    int speed_;
    BonusType kind_of_present_;
    bool is_on_screen_;

public:
	Present(float x, float y);
    Present(BonusType kind,float x, float y);
    ~Present();


    void set_rect_cordinate( float x,  float y);
    sf::FloatRect get_rect() const override;

    void set_is_on_screen(bool is_on_screen) { is_on_screen_ = is_on_screen; }
    bool get_is_on_screen() const { return is_on_screen_; }

	float get_x() const { return x; }
	float get_y() const { return y; }

    void set_kind_of_present(BonusType kind_of_present) {
        kind_of_present_ = kind_of_present;
        load_texture();
    }
    BonusType get_kind_of_present() const { return kind_of_present_; }

    void render(sf::RenderWindow& window);
    void update(float dt);
	BonusType get_kind() const;

private:
    bool load_texture();
};
#endif