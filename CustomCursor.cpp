// CustomCursor.cpp
#include "CustomCursor.h"
#include <iostream>

CustomCursor::CustomCursor() : visible(true) {
    sprite.setScale(0.1f, 0.1f);
}

bool CustomCursor::loadFromFile(const std::string& path) {
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load cursor: " << path << std::endl;
        return false;
    }
    sprite.setTexture(texture);

    sprite.setScale(0.08f, 0.08f);
    sprite.setColor(sf::Color::White);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    return true;
}

void CustomCursor::update(const sf::RenderWindow& window) {
    if (!visible) return; // nếu ẩn thì không cập nhật
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sprite.setPosition(mousePos);
}

void CustomCursor::draw(sf::RenderWindow& window) const {
    if (visible) {
        window.draw(sprite);
    }
}

void CustomCursor::setVisible(bool v) {
    visible = v;
}

bool CustomCursor::isVisible() const {
    return visible;
}
