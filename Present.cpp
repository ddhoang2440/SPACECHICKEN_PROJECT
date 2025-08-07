#include "Present.h"
#include "Entity.h"
// IMPLEMENTATION
Present::Present(): Entity(){
    name_ = BonusType(rand() % 4); // Chọn ngẫu nhiên một loại quà
    speed_ = 5.0f;                 // Tốc độ mặc định
    is_on_screen_ = true;

    // Tải texture dựa trên loại quà
    if (name_ == ATOMIC_POWER){
        texture_.loadFromFile("res/image/power_up.png");
    }else if (name_ == NEUTRON){
        texture_.loadFromFile("res/image/neutron_gift.png");
    }else if (name_ == ARROW){
        texture_.loadFromFile("res/image/arrow_gift.png");
    }else if (name_ == BORON){
        texture_.loadFromFile("res/image/boron_gift.png");
    }else if(name_ == SHIELD){
        texture_.loadFromFile("res/image/shield.png");
    }else if(name_ == LIFE){
        texture_.loadFromFile("res/image/heart.png");
    }
    anim_.sprite_.setTexture(texture_);
    anim_.sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
    anim_.sprite_.setScale(1.0f, 1.0f); // Tùy chỉnh nếu cần
}

Present::Present(BonusType name)
{
    name_ = name;
    speed_ = 5.0f;
    is_on_screen_ = true;

    // Tải texture dựa trên loại quà
    if (name_ == ATOMIC_POWER){
        texture_.loadFromFile("res/image/power_up.png");
    }else if (name_ == NEUTRON){
        texture_.loadFromFile("res/image/neutron_gift.png");
    }else if (name_ == ARROW){
        texture_.loadFromFile("res/image/arrow_gift.png");
    }else if (name_ == BORON){
        texture_.loadFromFile("res/image/boron_gift.png");
    }else if(name_ == SHIELD){
        texture_.loadFromFile("res/image/shield.png");
    }else if(name_ == LIFE){
        texture_.loadFromFile("res/image/heart.png");
    }
    anim_.sprite_.setTexture(texture_);
    anim_.sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
    anim_.sprite_.setScale(1.0f, 1.0f);
}

Present::~Present()
{
}

void Present::setName(BonusType name)
{
    name_ = name;
    if (name_ == ATOMIC_POWER){
        texture_.loadFromFile("res/image/power_up.png");
    }else if (name_ == NEUTRON){
        texture_.loadFromFile("res/image/neutron_gift.png");
    }else if (name_ == ARROW){
        texture_.loadFromFile("res/image/arrow_gift.png");
    }else if (name_ == BORON){
        texture_.loadFromFile("res/image/boron_gift.png");
    }else if(name_ == SHIELD){
        texture_.loadFromFile("res/image/shield.png");
    }else if(name_ == LIFE){
        texture_.loadFromFile("res/image/heart.png");
    }
    anim_.sprite_.setTexture(texture_);
    anim_.sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
}

void Present::update(float deltaTime)
{
    if (!is_on_screen_) return;

    // Di chuyển quà xuống dưới
    anim_.sprite_.move(0, speed_ * deltaTime);

    // Kiểm tra ra khỏi màn hình
    if (anim_.sprite_.getPosition().y > 1080) // Giả sử SCREEN_HEIGHT = 1080
    {
        is_on_screen_ = false;
    }
}

void Present::render(sf::RenderWindow& window)
{
    if (!is_on_screen_) return;

    // Điều chỉnh kích thước dựa trên loại quà
    if (name_ == ATOMIC_POWER)
        anim_.sprite_.setScale(40.0f / texture_.getSize().x, 50.0f / texture_.getSize().y);
    else
        anim_.sprite_.setScale(49.0f / texture_.getSize().x, 58.0f / texture_.getSize().y);

    window.draw(anim_.sprite_);
}
