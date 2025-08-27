#include "BackGround.h"
#include "CommonVariable.h"
#include <iostream>

BackGround::BackGround() : scrollSpeed_(20.f) {
    background1_.setPosition(0, 0);
    background2_.setPosition(0, 0);
}

void BackGround::loading_background(const std::string& file) {
    if (!texture_.loadFromFile(file)) {
        std::cerr << "Failed to load background image: " << file << std::endl;
        return;
    }
    background1_.setTexture(texture_);
    background2_.setTexture(texture_);
    float scaleX = static_cast<float>(SCREEN_WIDTH) / texture_.getSize().x;
    float scaleY = static_cast<float>(SCREEN_HEIGHT) / texture_.getSize().y;
    background1_.setScale(scaleX, scaleY);
    background2_.setScale(scaleX, scaleY);
    background1_.setPosition(0, 0);
    background2_.setPosition(0, -static_cast<float>(texture_.getSize().y * scaleY));
}

void BackGround::update_background_scroll() {
    background1_.move(0, scrollSpeed_ * 0.016f); // Giả sử 60 FPS
    background2_.move(0, scrollSpeed_ * 0.016f);
    if (background1_.getPosition().y >= SCREEN_HEIGHT) {
        background1_.setPosition(0, background2_.getPosition().y - texture_.getSize().y * background1_.getScale().y);
    }
    if (background2_.getPosition().y >= SCREEN_HEIGHT) {
        background2_.setPosition(0, background1_.getPosition().y - texture_.getSize().y * background2_.getScale().y);
    }
}

void BackGround::render_background_scroll(sf::RenderWindow& window) {
    window.draw(background1_);
    window.draw(background2_);
}