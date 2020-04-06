#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <allegro5/allegro.h>
#include <iostream>

#include "Constants.h"

class Background
	{
	public:
		Background(std::string path, float moveSpeed);
		void init();
		void change(std::string path, float moveSpeed);
		void update();
		void render();
		void dispose();
		
	private:
		std::string image_path;
		float move_speed;
		ALLEGRO_BITMAP* image;
		float y;
	};

#endif // BACKGROUND_H
