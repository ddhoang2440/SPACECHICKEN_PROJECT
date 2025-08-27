#pragma once
#include <SFML/Graphics.hpp>

class BackGround {
private:
    sf::Texture texture_;
    sf::Sprite background1_;
    sf::Sprite background2_;
    float scrollSpeed_;

public:
    BackGround();
    void loading_background(const std::string& file);
    void update_background_scroll();
    void render_background_scroll(sf::RenderWindow& window);
};
