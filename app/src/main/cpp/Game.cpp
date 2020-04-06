#include "Game.h"
#include <iostream>

#include "Constants.h"
#include "State.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_android.h>

Game::Game() {
	init();
}

void Game::init() {
	assert(al_init());
    
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SUPPORTED_ORIENTATIONS, ALLEGRO_DISPLAY_ORIENTATION_PORTRAIT, ALLEGRO_REQUIRE);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	
	display = al_create_display(GAME_WIDTH, GAME_HEIGHT);
	assert(display);
	event_queue = al_create_event_queue();
	assert(event_queue);
	timer = al_create_timer(1.0/60);
	assert(timer);
	
	// Initialize addons
	assert(al_init_image_addon());
    assert(al_init_font_addon());
    assert(al_init_ttf_addon());
    assert(al_install_audio());
    assert(al_init_acodec_addon());
    
    assert(al_reserve_samples(20));
	
	assert(al_install_keyboard());

#ifndef ALLEGRO_ANDROID
	assert(al_install_mouse());
#else
	assert(al_install_touch_input());
#endif

	assert(al_init_primitives_addon());
	
	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

#ifndef ALLEGRO_ANDROID
	al_register_event_source(event_queue, al_get_mouse_event_source());
#else
	al_register_event_source(event_queue, al_get_touch_input_event_source());
	al_android_set_apk_file_interface();
#endif
	
	running = true;
	redraw = false;
	
	paused_state = nullptr;
    current_state = nullptr;

    scaleDisplay();
}

void Game::update() {
	current_state->update();
}

void Game::handleInput() {
	current_state->handleInput(event);
}

void Game::render() {
	al_clear_to_color(al_map_rgb_f(0, 0, 0));
	
	current_state->render();
	
	al_flip_display();
}

void Game::dispose() {
	
	current_state->dispose();
	delete current_state;
	current_state = nullptr;
	if (paused_state) {
		paused_state->dispose();
		delete paused_state;
		paused_state = nullptr;
	}
	
	al_destroy_display(display);
	display = nullptr;
	al_destroy_timer(timer);
	timer = nullptr;
	al_destroy_event_queue(event_queue);
	event_queue = nullptr;
}

void Game::scaleDisplay() {
	ALLEGRO_TRANSFORM scaleTransform;
	al_identity_transform(&scaleTransform);
	al_scale_transform(&scaleTransform, al_get_display_width(al_get_current_display()) / (float)GAME_WIDTH, al_get_display_height(al_get_current_display()) / (float) GAME_HEIGHT);

	al_use_transform(&scaleTransform);
}

void Game::transformCoordinate(int* x, int* y) {
	int dw = al_get_display_width(al_get_current_display());
	int dh = al_get_display_height(al_get_current_display());

	float scaleX = dw / (float) GAME_WIDTH;
	float scaleY = dh / (float) GAME_HEIGHT;

	int tx = *x;
	int ty = *y;

	if (dw >= GAME_WIDTH) { // The window is larger than our game world
		*x = static_cast<int>(tx / (scaleX >= 1 ? scaleX : 1)); // We make sure to not divide by zero
	} else {
		float temp = ((float) GAME_WIDTH / dw); // Invert the division
		*x = static_cast<int>(tx * (temp >= 1 ? temp : 1));
	}

	if (dh >= GAME_HEIGHT) {
		*y = static_cast<int>(ty / (scaleY >= 1 ? scaleY : 1));
	} else {
		float temp = ((float)GAME_HEIGHT / dh);
		*y = static_cast<int>(ty * (temp >= 1 ? temp : 1));
	}
}

void Game::startGame() {
	al_start_timer(timer);
	while (running) {
		al_wait_for_event(event_queue, &event);
		
		switch (event.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;
			case ALLEGRO_EVENT_TIMER:
				update();
				handleInput();
				redraw = true;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
			case ALLEGRO_EVENT_KEY_UP:
			case ALLEGRO_EVENT_KEY_CHAR:
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			case ALLEGRO_EVENT_MOUSE_AXES:
			case ALLEGRO_EVENT_TOUCH_BEGIN:
			case ALLEGRO_EVENT_TOUCH_END:
			case ALLEGRO_EVENT_TOUCH_MOVE:
				handleInput();
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(display);
				scaleDisplay();
				break;
			case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
				al_acknowledge_drawing_halt(display);
				al_flush_event_queue(event_queue);
				current_state->pause();
				al_stop_timer(timer);
				break;
			case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING:
				al_acknowledge_drawing_resume(display);
				current_state->resume();
				al_start_timer(timer);
				break;
			case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
				current_state->pause();
				break;
			case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
				current_state->resume();
				break;
		}
		
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			render();
		}
	}
	
	dispose();
}

void Game::pauseState(State *newState) {
	newState->init();
	paused_state = current_state;
	current_state = newState;
}

void Game::resumeState() {
	current_state->dispose();
	delete current_state;
	current_state = nullptr;
	
	current_state = paused_state;
	current_state->resume();
	paused_state = nullptr;
}

void Game::changeState(State *newState) {
    // Save the current state in a temporary variable for deallocation
	State* temp = nullptr;
	if (current_state) {
        temp = current_state;
    }
    
    // Assign the new state
	current_state = newState;
    current_state->init();
    
    // Now deallocate the temporary state
	if (temp) {
		temp->dispose();
		delete temp;
	}
    
    // if there was a previously paused state, remove that too
    if (paused_state) {
        paused_state->dispose();
        delete paused_state;
        paused_state = nullptr;
    }
    
}

