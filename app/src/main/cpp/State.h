#ifndef STATE_H
#define STATE_H
#include <allegro5/allegro.h>

class Game; // Forward declaration

class State {
	public:
		State(Game &game);
		virtual void init() = 0;
		virtual void update() = 0;
		virtual void handleInput(ALLEGRO_EVENT& event) = 0;
		virtual void render() = 0;
		virtual void dispose() = 0;
		virtual void pause();
		virtual void resume();
		virtual ~State() = default;
	protected:
		Game& game;
	};

#endif // STATE_H
