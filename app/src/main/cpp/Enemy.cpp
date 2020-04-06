#include "Enemy.h"
#include "Constants.h"

Enemy::Enemy(int rank_) : rank(rank_) {
    init();
}

void Enemy::init() {
    dead = false;
    
    // Set enemy information based on the rank
    switch (rank) {
    
    case 1:
        lives = 3;
        speed = 1 + rand () % 3;
        image_path = "res/enemy1.png";
        break;
    case 2:
        lives = 5;
        speed = 1 + rand () % 3;
        image_path = "res/enemy2.png";
        break;
    case 3:
        lives = 6;
        speed = 1 + rand () % 4;
        image_path = "res/enemy3.png";
        break;
    case 4:
        lives = 8;
        speed = 1 + rand () % 5;
        image_path = "res/enemy4.png";
        break;
    case 5:
        lives = 10;
        speed = 1 + rand () % 5;
        image_path = "res/enemy5.png";
        break;
    }
    
    image = al_load_bitmap(image_path.c_str());
    assert(image);
    width = al_get_bitmap_width(image);
    height = al_get_bitmap_height(image);
    x = rand() % static_cast<int>(GAME_WIDTH - width);
    y = -height;
    dx = 0;
    dy = speed;
}

void Enemy::update() {
    x += dx;
    y += dy;
    
    if (rank > 3) {
        int random = rand() % 50;
        if (random == 5) {
            Bullet* bullet = new Bullet(1, "res/enemy_bullet.png", 90, x + width / 2, y + height / 2);
            bullet->setFlip(ALLEGRO_FLIP_VERTICAL);
            bullets.emplace_back(bullet);
        }
    }
    
    for (auto& bullet : bullets) {
        bullet->update();
    }
    
    for (auto bullet = bullets.begin(); bullet != bullets.end(); bullet++) {
        if ((*bullet)->isDead()) {
            (*bullet)->dispose();
            delete (*bullet);
            bullets.erase(bullet);
            bullet--;
        }
    }
    
    dead = x < 0 || x > GAME_WIDTH - width || y > GAME_HEIGHT || lives <= 0;
}

void Enemy::render() {
    al_draw_bitmap(image, x, y, ALLEGRO_FLIP_VERTICAL);
    for (auto& bullet : bullets) {
        bullet->render();
    }
}

void Enemy::dispose() {
    al_destroy_bitmap(image);
    image = nullptr;
    
    for (auto& bullet : bullets) {
        bullet->dispose();
        delete bullet;
    }
    bullets.clear();
}
