#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <cmath>
using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;
class Animation
{
public:
    float frame_;
    float speed_;
    sf::Sprite sprite_;
    std::vector<sf::IntRect> frames_;

    Animation() : frame_(0), speed_(0) {}

    Animation(sf::Texture& t, int x, int y, int w, int h, int count, float speed)
    {
        frame_ = 0;
        speed_ = speed;
        for (int i = 0; i < count; i++)
            frames_.push_back(sf::IntRect(x + i * w, y, w, h));
        sprite_.setTexture(t);
        sprite_.setOrigin(w / 2.0f, h / 2.0f);
        sprite_.setTextureRect(frames_[0]);
    }

    void update()
    {
        frame_ += speed_;
        int n = frames_.size();
        if (frame_ >= n) frame_ -= n;
        if (n > 0) sprite_.setTextureRect(frames_[static_cast<int>(frame_)]);
    }

    bool isEnd()
    {
        return frame_ + speed_ >= frames_.size();
    }
};