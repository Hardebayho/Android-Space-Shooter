#include "Background.h"

Background::Background(std::string path, float moveSpeed) : image_path(path), move_speed(moveSpeed) {
	init();
}

void Background::init() {
	image = al_load_bitmap(image_path.c_str());
	assert(image);
	y = 0;
}

void Background::update() {
	y -= move_speed;
	if (y < -GAME_HEIGHT) y = 0;
}

void Background::render() {
	al_draw_scaled_bitmap(image, 0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image), 0, y, GAME_WIDTH, GAME_HEIGHT, 0);
	al_draw_scaled_bitmap(image, 0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image), 0, y + GAME_HEIGHT, GAME_WIDTH, GAME_HEIGHT, 0);
}

void Background::change(std::string path, float moveSpeed) {
	image_path = path;
	move_speed = moveSpeed;
	dispose();
	init();
}

void Background::dispose() {
	al_destroy_bitmap(image);
	image = nullptr;
}
