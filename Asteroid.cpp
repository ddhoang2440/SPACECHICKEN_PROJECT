#include "Asteroid.h"
#include <iostream>
#include <cstdlib>
#include <string>

Asteroid::Asteroid()
    : Entity(),
    random_type(3 + (std::rand() % 3)),
    v_x(std::rand() % 5 - 2),
    v_y(std::rand() % 5 + 2),
    speed_(8),
    health_(3),
    is_on_screen(true),
    spinning_angle_(0),
    size_level_(2),
    nFrames_(1) // mặc định 1 frame
{
    x = static_cast<float>(std::rand() % SCREEN_WIDTH);
    y = -100.f - static_cast<float>(std::rand() % 500);

    std::string type = "res/image/asteroid-" + std::to_string(random_type) + ".png";
    if (!texture_.loadFromFile(type)) {
        std::cout << "Failed to load texture: " << type << std::endl;
        return;
    }

    // Lấy size texture
    sf::Vector2u texSize = texture_.getSize();
    original_width_ = static_cast<float>(texSize.x);
    original_height_ = static_cast<float>(texSize.y);

    // Tự động phân biệt sprite sheet hay ảnh đơn
    if (original_width_ >= original_height_ * 2) {
        // Giả sử sprite sheet nhiều frame ngang, bạn có thể thay đổi số frame ở đây
        nFrames_ = 3;
    }
    else {
        nFrames_ = 1; // ảnh đơn
    }

    frame_width_ = original_width_ / nFrames_;
    frame_height_ = original_height_;

    // >>> THAY ĐỔI 1: KHỞI TẠO ANIMATION CHỈ MỘT LẦN VỚI KÍCH THƯỚC FRAME ĐẦY ĐỦ
    // Khởi tạo animation với kích thước đầy đủ của một frame
    anim_ = Animation(texture_, 0, 0, frame_width_, frame_height_, nFrames_, 0.05f); // Tăng tốc độ anim một chút

    // >>> THAY ĐỔI 2: ĐẶT TÂM XOAY (ORIGIN) VỀ GIỮA SPRITE
    // Điều này CỰC KỲ QUAN TRỌNG để sprite xoay và scale đúng tâm
    anim_.sprite_.setOrigin(frame_width_ / 2.f, frame_height_ / 2.f);

    // Đặt vị trí ban đầu
    anim_.sprite_.setPosition(sf::Vector2f(x, y));

    // Gọi set_size_level để áp dụng scale ban đầu
    set_size_level(size_level_);
}

Asteroid::~Asteroid() { free(); }

void Asteroid::set_size_level(int lvl)
{
    size_level_ = lvl;
    float scale = 1.0f; // Tỉ lệ scale, mặc định là 100%

    switch (lvl)
    {
    case 0: // Nhỏ
        health_ = 1;
        scale = 0.5f;
        break;
    case 1: // Vừa
        health_ = 2;
        scale = 0.75f;
        break;
    case 2: // Lớn
    default:
        health_ = 3;
        scale = 1.0f;
        break;
    }

    anim_.sprite_.setScale(scale, scale);


    width_ = frame_width_ * scale;
    height_ = frame_height_ * scale;


    radius_ = std::max(width_, height_) / 2.f;
}

void Asteroid::set_rect_cordinate(const float& x, const float& y)
{
    this->x = x;
    this->y = y;
}

sf::FloatRect Asteroid::get_rect() const
{
    return sf::FloatRect(x - width_ / 2.f,
        y - height_ / 2.f,
        width_,
        height_);
}

void Asteroid::draw(sf::RenderWindow& window)
{
    if (!is_on_screen || health_ <= 0) return;

    anim_.sprite_.setPosition(sf::Vector2f(x, y));
    anim_.sprite_.setRotation(static_cast<float>(spinning_angle_));
    window.draw(anim_.sprite_);
}

void Asteroid::update()
{
    if (!is_on_screen || health_ <= 0) return;

    moving_diagonal();
    spinning();
    anim_.update(); 
}

void Asteroid::moving_diagonal()
{
    if (!is_on_screen || health_ <= 0) return;


    const float dt = 1.0f / 60.0f;
    x += v_x * speed_ * 10.f * dt;
    y += v_y * speed_ * 10.f * dt;

    // Respawn nếu đi ra ngoài màn hình
    if (y > SCREEN_HEIGHT + height_) { // dùng height_ để chắc chắn nó ra khỏi màn hình hoàn toàn
        is_on_screen = false; // Tạm thời tắt đi để logic game xử lý việc hồi sinh
        // Logic hồi sinh có thể được quản lý bởi một lớp cấp cao hơn
        x = static_cast<float>(std::rand() % SCREEN_WIDTH);
        y = -height_ - static_cast<float>(std::rand() % 300);
        v_x = static_cast<float>(std::rand() % 5 - 2);
        v_y = static_cast<float>(std::rand() % 5 + 2);
        is_on_screen = true;
    }
}

void Asteroid::spinning()
{
    if (!is_on_screen || health_ <= 0) return;

    spinning_angle_ += 2.0f; // Giảm tốc độ xoay một chút cho đẹp hơn
    if (spinning_angle_ > 360.f) spinning_angle_ -= 360.f;
}

void Asteroid::free()
{
    // Không cần làm gì nhiều ở đây vì texture và animation sẽ tự giải phóng
}
