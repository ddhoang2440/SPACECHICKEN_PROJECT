// #include"Players.h"
// #include"Chicken.h"

// const int SCREEN_WIDTH = 1200;
// const int SCREEN_HEIGHT = 800;
// const float DEGTORAD = 0.017453f;
// const int PLAYERS_NUMS_FRAME = 4;
// const float PLAYERS_spritetime = 100.0f; // ms

// Players:: Players():Entity(){
//     // texture_.loadFromFile("spaceship.png");
// 	// sprite_.setTexture(texture_);
//     name_ = "Player";
//     life_=true;
//     number_of_wings=0;
//     slow_move=false;
//     health=3;
//     ammo_type=1;
//     ammo_level=0;
//     shots_=1;
//     width_of_sprite_ = 0;
//     height_of_sprite_ = 0;
//     immunity_ = 0;

//     Score = 0;

//     load_animation_sprite("res/image/spaceship.png");
//     // d.loadFromFile("Ci1playerhit.oga");
//     // die.setBuffer(d);
//     // o.loadFromFile("Ci1gameover.oga");
//     // gameover.setBuffer(o);
//     // Tải âm thanh
//     if (!shoot_buffer.loadFromFile("res/sound/arrow_firing.wav")) {
//         std::cerr << "Error: Could not load arrow_firing.wav" << std::endl;
//     }
//     shoot_sound.setBuffer(shoot_buffer);

//     if (!eat_wing_buffer.loadFromFile("res/sound/GET_FOOD.wav")) {
//         std::cerr << "Error: Could not load GET_FOOD.wav" << std::endl;
//     }
//     eat_wing_sound.setBuffer(eat_wing_buffer);

//     if (!hit_buffer.loadFromFile("res/sound/player_hit.wav")) {
//         std::cerr << "Error: Could not load player_hit.wav" << std::endl;
//     }
//     hit_sound.setBuffer(hit_buffer);

//     if (!present_buffer.loadFromFile("res/sound/get_weapon.wav")) {
//         std::cerr << "Error: Could not load get_weapon.wav" << std::endl;
//     }
//     getting_present_sound.setBuffer(present_buffer);
// }
//     Players::~Players()
//     {
//         free();
//     }
//     void Players::free()
//     {
//         for (auto& bullet : ammo_list) {
//             delete bullet;
//         }
//         ammo_list.clear();

//         for (auto& explosion : explosion_list) {
//             delete explosion;
//         }
//         explosion_list.clear();
//     }
//     void Players::set_clips()
//     {
//         if (width_of_sprite_ >= 0 && height_of_sprite_ >= 0)
//         {
//             anim_.frames_.clear();
//             for (int i = 0; i < PLAYERS_NUMS_FRAME; i++)
//             {
//                 anim_.frames_.push_back(sf::IntRect(i * width_of_sprite_, 0, width_of_sprite_, height_of_sprite_));
//             }
//             anim_.sprite_.setTextureRect(anim_.frames_[0]);
//         }
//     }

//     void Players::load_animation_sprite(const std::string& file)
//     {
//         if (!texture_.loadFromFile(file)) {
//             std::cerr << "Error: Could not load " << file << std::endl;
//             return;
//         }
//         width_of_sprite_ = texture_.getSize().x / PLAYERS_NUMS_FRAME;
//         height_of_sprite_ = texture_.getSize().y;
//         anim_ = Animation(texture_, 0, 0, width_of_sprite_, height_of_sprite_, PLAYERS_NUMS_FRAME, PLAYERS_spritetime / 1000.0f);
//         x = SCREEN_WIDTH / 2.0f;
//         y = SCREEN_HEIGHT - 100;
//         anim_.sprite_.setPosition(x, y);
//     }

//     void Players::render_animation(sf::RenderWindow& window, float scale)
//     {
//         if (health <= 0) return;
//         if (slow_move)
//         {
//             slowly_move_from_bottom();
//             return;
//         }

//         update();
//         anim_.sprite_.setScale(scale, scale);
//         window.draw(anim_.sprite_);
//     }

//     void Players::update()
//     {
//         anim_.update();
//         if (immunity_ && immunity_timer_.getElapsedTime().asSeconds() > 5.0f)
//         {
//             immunity_ = false; // Tắt miễn nhiễm sau 5 giây
//         }
//     }

//     void Players::draw(sf::RenderWindow& window)
//     {
//         render_animation(window, 1.0f);
//     }

//     void Players::set_rect_cordinate(float x, float y)
//     {
//         this->x = x;
//         this->y = y;
//         anim_.sprite_.setPosition(x, y);
//     }

//     void Players::set_rect_cordinate_width_and_height(float x, float y, float width, float height)
//     {
//         this->x = x;
//         this->y = y;
//         width_of_sprite_ = width;
//         height_of_sprite_ = height;
//         set_clips();
//     }

//     void Players::set_rect_width_and_height(float width, float height)
//     {
//         width_of_sprite_ = width;
//         height_of_sprite_ = height;
//         set_clips();
//     }

//     sf::FloatRect Players::get_rect() const
//     {
//         return sf::FloatRect(x, y, width_of_sprite_, height_of_sprite_);
//     }

//     sf::FloatRect Players::get_rect_width_height_with_scale(float scale) const
//     {
//         return sf::FloatRect(x, y, width_of_sprite_ * scale, height_of_sprite_ * scale);
//     }

//     void Players::handling_movement(sf::Event& event)
//     {
//         if (health <= 0 || slow_move) return;
//         if (y >= 901) slow_move = false;

//         // Tốc độ di chuyển (tương tự movingship)
//         float speed = 0.75f;

//         // Kích thước sprite
//         float sprite_width = texture_.getSize().x / (float)PLAYERS_NUMS_FRAME;
//         float sprite_height = texture_.getSize().y;

//         float deltaTime = event_clock_.restart().asSeconds(); // eventClock là sf::Clock
//         float speed = 45.0f; // 0.75 pixel/frame * 60 FPS = 45 pixel/giây
//         // Kiểm tra phím mũi tên và WASD
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//         {
//             if (x < 1920 - sprite_width) x += speed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//         {
//             if (x > 0) x -= speed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//         {
//             if (y < 1080 - sprite_height) y += speed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//         {
//             if (y > 0) y -= speed;
//         }

//         // Cập nhật vị trí sprite
//         anim_.sprite_.setPosition(x, y);
//     }
//     void Players::handling_shooting(sf::Event& event)
//     {
//         if (health <= 0 || slow_move) return;

//         // Bắn khi nhấn Space hoặc chuột trái, với tần suất 0.4 giây
//         if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) ||
//             (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left))
//         {
//             if (shoot_timer_.getElapsedTime().asSeconds() > 0.4f)
//             {
//                 std::string path = "res/image/";
//                 std::string ammo_type_str;
//                 Bullet::BulletType bullet_type;
//                 int damage;

//                 // Xác định loại đạn và sát thương
//                 if (ammo_type == 2) {
//                     ammo_type_str = "BORON";
//                     bullet_type = Bullet::BORON;
//                     damage = 2 + ammo_level;
//                 } else if (ammo_type == 1) {
//                     ammo_type_str = "NEUTRON";
//                     bullet_type = Bullet::NEURON;
//                     damage = 1 + ammo_level;
//                 } else if (ammo_type == 3) {
//                     ammo_type_str = "arrow";
//                     bullet_type = Bullet::ARROW;
//                     damage = 1 + ammo_level;
//                 } else {
//                     return; // Không bắn nếu ammo_type không hợp lệ
//                 }

//                 // Tạo số lượng đạn dựa trên shots_
//                 for (int i = 0; i < shots_; i++)
//                 {
//                     Bullet* bullet = nullptr;
//                     if (bullet_type == Bullet::BORON)
//                         bullet = new BoronBullet();
//                     else if (bullet_type == Bullet::NEURON)
//                         bullet = new NeuronBullet();
//                     else
//                         bullet = new ArrowBullet();

//                     std::string path_ammo = path + ammo_type_str + (ammo_type != 3 ? std::to_string(ammo_level) : "") + ".png";
//                     if (!bullet->load_static_bullet_picture(path_ammo)) {
//                         std::cerr << "Error: Could not load " << path_ammo << std::endl;
//                         delete bullet;
//                         continue;
//                     }

//                     bullet->set_alive(true);
//                     bullet->set_can_move(true);
//                     bullet->set_speed(ammo_type == 2 ? 10.0f : 20.0f);
//                     bullet->set_damage(damage);

//                     // Đặt vị trí và góc xoay
//                     float sprite_width = texture_.getSize().x / (float)PLAYERS_NUMS_FRAME;
//                     float base_x = x + sprite_width / 2.0f;
//                     float base_y = y - 50;
//                     if (shots_ == 1) {
//                         bullet->setpos(base_x, base_y); 
//                     } else if (shots_ == 2) {
//                         bullet->setpos(base_x + (i == 0 ? 23 : -23), base_y);
//                     } else if (shots_ == 3) {
//                         if (i == 0) bullet->setrotation(20);
//                         else if (i == 1) bullet->setrotation(-20);
//                         else bullet->setpos(base_x, base_y);
//                     }

//                     ammo_list.push_back(bullet);
//                 }

//                 shoot_sound.setVolume(12);
//                 shoot_sound.play();
//                 shoot_timer_.restart();
//             }
//         }
//     }

//     void Players::render_shooting()
//     {
//         if (health <= 0 || slow_move) return;

//         float deltaTime = 1.0f / 60.0f; // Giả định 60 FPS
//         for (auto it = ammo_list.begin(); it != ammo_list.end();)
//         {
//             if ((*it)->is_alive())
//             {
//                 // Di chuyển đạn
//                 if (shots_ < 3) {
//                     (*it)->setpos((*it)->getSprite().getPosition().x, (*it)->getSprite().getPosition().y - 1);
//                 } else {
//                     float rotation = (*it)->getrotation();
//                     if (rotation == 20)
//                         (*it)->setpos((*it)->getSprite().getPosition().x + 0.447f, (*it)->getSprite().getPosition().y - 1);
//                     else if (rotation == 340)
//                         (*it)->setpos((*it)->getSprite().getPosition().x - 0.447f, (*it)->getSprite().getPosition().y - 1);
//                     else
//                         (*it)->setpos((*it)->getSprite().getPosition().x, (*it)->getSprite().getPosition().y - 1);
//                 }

//                 (*it)->render(*window_); // Giả định Players có con trỏ sf::RenderWindow* window
//                 ++it;
//             }
//             else
//             {
//                 delete *it;
//                 it = ammo_list.erase(it);
//             }
//         }
//     }
//     void Players::process_shooting_if_hit_chicken(Chicken* chicken)
//     {
//         if (health <= 0 || chicken->get_health() <= 0) return;

//         for (auto& ammo : ammo_list)
//         {
//             if (check_collision_2_rect(ammo->get_rect(), chicken->get_rect()))
//             {
//                 chicken->play_hit_sound();
//                 chicken->set_health(chicken->get_health() - ammo->get_damage());
//                 if (chicken->get_health() <= 0)
//                 {
//                     chicken->set_rect_cordinate(-9999, -9999);
//                     chicken->set_speed(0);
//                     chicken->set_alive(false);
//                 }
//                 ammo->set_alive(false);
//                 ammo->set_speed(0);
//                 ammo->set_can_move(false);
//                 return;
//             }
//         }
//     }
//     void Players::process_if_eat_wing_rect(Chicken* chicken)
//     {
//         if (health <= 0) return;

//         sf::FloatRect player_rect(x, y, texture_.getSize().x / PLAYERS_NUMS_FRAME, texture_.getSize().y);
//         if (chicken->get_health() <= 0 && check_collision_2_rect(chicken->get_wing_rect(), player_rect) &&
//             chicken->get_on_screen() && chicken->get_has_wing())
//         {
//             number_of_wings++;
//             eat_wing_sound.play();
//             chicken->set_on_screen(false);
//         }
//     }
//     void Players::process_if_hit_by_chicken(Chicken* chicken)
//     {
//         if (health <= 0) return;

//         sf::FloatRect player_rect(x, y, texture_.getSize().x / PLAYERS_NUMS_FRAME, texture_.getSize().y);
//         if (chicken->get_health() >= 1 && check_collision_2_rect(chicken->get_rect(), player_rect) &&
//             chicken->get_on_screen() && !slow_move)
//         {
//             Explosion* explosion = new Explosion();
//             explosion->set_is_on_screen(true);
//             explosion->load_animation_sprite("res/image/explosion.png");
//             explosion->set_clips();
//             explosion->set_coordinates(x, y);
//             explosion_list.push_back(explosion);

//             hit_sound.setVolume(64);
//             hit_sound.play();
//             chicken->set_health(-1);
//             chicken->set_on_screen(false);
//             chicken->set_alive(false);
//             health--;
//             set_slow_move();
//         }
//     }
//     void Players::process_if_hit_by_eggs(Chicken* chicken)
//     {
//         if (health <= 0) return;

//         sf::FloatRect player_rect(x, y, texture_.getSize().x / PLAYERS_NUMS_FRAME, texture_.getSize().y);
//         for (auto& egg : chicken->get_eggs_list())
//         {
//             if (check_collision_2_rect(egg->get_rect(), player_rect) && !egg->get_is_broken() && egg->get_alive())
//             {
//                 Explosion* explosion = new Explosion();
//                 explosion->set_is_on_screen(true);
//                 explosion->load_animation_sprite("res/image/explosion.png");
//                 explosion->set_clips();
//                 explosion->set_coordinates(x, y);
//                 explosion_list.push_back(explosion);

//                 hit_sound.setVolume(80);
//                 hit_sound.play();
//                 health--;
//                 egg->set_alive(false);
//                 set_slow_move();
//                 return;
//             }
//         }
//     }
//     void Players::process_if_hit_by_asteroid(Asteroid* asteroid)
//     {
//         if (health <= 0) return;

//         sf::FloatRect player_rect(x, y, texture_.getSize().x / PLAYERS_NUMS_FRAME, texture_.getSize().y);
//         if (check_collision_2_rect(asteroid->get_rect(), player_rect) && asteroid->get_is_on_screen())
//         {
//             health--;
//             hit_sound.setVolume(80);
//             hit_sound.play();

//             Explosion* explosion = new Explosion();
//             explosion->set_is_on_screen(true);
//             explosion->load_animation_sprite("res/image/explosion.png");
//             explosion->set_clips();
//             explosion->set_coordinates(x, y);
//             explosion_list.push_back(explosion);

//             asteroid->set_health(-1);
//             asteroid->set_is_on_screen(false);
//             asteroid->set_rect_cordinate(-9999, -9999);
//             set_slow_move();
//         }
//         for (auto& ammo : ammo_list)
//         {
//             if (check_collision_2_rect(ammo->get_rect(), asteroid->get_rect()) && ammo->get_can_move() && asteroid->get_is_on_screen())
//             {
//                 asteroid->set_health(asteroid->get_health() - 1);
//                 if (asteroid->get_health() <= 0)
//                 {
//                     asteroid->set_rect_cordinate(-9999, -9999);
//                     asteroid->set_is_on_screen(false);
//                 }
//                 ammo->set_can_move(false);
//                 ammo->set_alive(false);
//                 return;
//             }
//         }
//     }
//     void Players::processing_if_got_present(Present* pickup)
//     {
//         if (health <= 0 || !pickup) return;

//         sf::FloatRect player_rect(x, y, width_of_sprite_, height_of_sprite_);
//         if (check_collision_2_rect(pickup->get_rect(), player_rect) && pickup->get_is_on_screen())
//         {
//             getting_present_sound.play();
//             pickup->set_is_on_screen(false);

//             if (pickup->getName() == ATOMIC_POWER || pickup->getName() == LIFE)
//             {
//                 if (health < 10) health++;
//             }
//             else if (pickup->getName() == NEUTRON || pickup->getName() == BORON || pickup->getName() == ARROW)
//             {
//                 ammo_type = (pickup->getName() == NEUTRON) ? 1 : (pickup->getName() == BORON) ? 2 : 3;
//                 if (ammo_level < 3) ammo_level++;
//                 if (shots_ < 3) shots_++; // Tăng số lượng đạn bắn
//             }
//             else if (pickup->getName() == SHIELD)
//             {
//                 immunity_ = true;
//                 immunity_timer_.restart();
//             }
//         }
//     }
//     void Players::processing_if_hit_by_boss(Boss* boss)
//     {
//         if (health <= 0 || boss->get_health() <= 0) return;

//         sf::FloatRect player_rect(x, y, texture_.getSize().x / PLAYERS_NUMS_FRAME, texture_.getSize().y);
//         if (check_collision_2_rect(boss->get_rect(), player_rect) && !slow_move)
//         {
//             Explosion* explosion = new Explosion();
//             explosion->set_is_on_screen(true);
//             explosion->load_animation_sprite("res/image/explosion.png");
//             explosion->set_clips();
//             explosion->set_coordinates(x - 15, y - 15);
//             explosion_list.push_back(explosion);

//             hit_sound.play();
//             health--;
//             set_slow_move();
//         }

//         for (auto& ammo : ammo_list)
//         {
//             if (check_collision_2_rect(ammo->get_rect(), boss->get_rect()) && ammo->get_can_move())
//             {
//                 boss->set_health(boss->get_health() - ammo->get_damage());
//                 ammo->set_alive(false);
//                 ammo->set_can_move(false);
//                 hit_sound.play();
//                 return;
//             }
//         }
//     }
//     void Players::processing_if_hit_by_boss_egg(Boss* boss)
//     {
//         if (health <= 0) return;

//         sf::FloatRect player_rect(x, y, texture_.getSize().x / PLAYERS_NUMS_FRAME, texture_.getSize().y);
//         for (auto& egg : boss->get_egg_list())
//         {
//             if (check_collision_2_rect(egg->get_rect(), player_rect) && egg->get_alive())
//             {
//                 Explosion* explosion = new Explosion();
//                 explosion->set_is_on_screen(true);
//                 explosion->load_animation_sprite("res/image/explosion.png");
//                 explosion->set_clips();
//                 explosion->set_coordinates(x, y);
//                 explosion_list.push_back(explosion);

//                 hit_sound.setVolume(80);
//                 hit_sound.play();
//                 health--;
//                 egg->set_alive(false);
//                 set_slow_move();
//                 return;
//             }
//         }
//     }
