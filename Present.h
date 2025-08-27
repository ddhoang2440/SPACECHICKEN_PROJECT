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
    Present();
    Present(BonusType kind);
    ~Present();

    void set_rect_cordinate(const float& x, const float& y);
    sf::FloatRect get_rect() const override;

    void set_is_on_screen(bool is_on_screen) { is_on_screen_ = is_on_screen; }
    bool get_is_on_screen() const { return is_on_screen_; }

    void set_kind_of_present(BonusType kind_of_present) {
        kind_of_present_ = kind_of_present;
        load_texture();
    }
    BonusType get_kind_of_present() const { return kind_of_present_; }

    void render(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) override;
    void update() override;

private:
    void load_texture();
};
#endif