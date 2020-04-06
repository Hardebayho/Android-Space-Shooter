#ifndef GAME_H
#define GAME_H
#include <allegro5/allegro.h>

class State;

class Game {
public:
    Game();
    void init();
    void update();
    void handleInput();
    void render();
    void dispose();
    void startGame();

    void scaleDisplay();
    void transformCoordinate(int* x, int* y);
    
    /// Quits the game
    void quit() { running = false; }
	
	/// Pauses the current state, and makes the newState the current state
	void pauseState(State* newState);
	
	/// Deletes the current state and resumes the previous state
	void resumeState();
	
	/// Destroys the current state and changes to a new state
	void changeState(State* newState);

private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;
	
	bool running;
	bool redraw;
	ALLEGRO_EVENT event;
	
	State* current_state;
	State* paused_state;

};

#endif // GAME_H
