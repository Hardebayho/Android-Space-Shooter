#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "State.h"
#include "Background.h"

#include "Player.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Button.h"
#include "Powerup.h"

class PlayState : public State {
	public:
		PlayState (Game& game_, int _level, int playerBulletDamage = 1);
		void init() override;
		void update() override;
		void render() override;
		void handleInput(ALLEGRO_EVENT& event) override;
		void dispose() override;
		void pause() override;
		void resume() override;
        
        void spawnEnemies();
		
	private:
		Background* bg;
        int level;
        int score;
        
        /// The Level configuration handle
        ALLEGRO_CONFIG* level_config;
        std::string level_name;
        int num_enemies;
        
        // Starting the levels
        bool level_start; // Lets us know if it's time to start the level
        ALLEGRO_FONT* level_title_font;
        float level_title_x;
        float level_title_y;
        ALLEGRO_TIMER* level_start_timer;
        ALLEGRO_EVENT_QUEUE* level_start_event_queue;
        ALLEGRO_EVENT level_start_event;
        bool finishAnimation;
        
        bool levelCompleted;
        float level_finished_timer;
        float level_finished_timer_diff;
        
        // Acts
        int num_acts;
        int currentAct;
        int dead_enemies;
        
        std::vector<Enemy*> enemies;
        
        // Game Objects
        Player player;
        int currentPlayerBulletDamage;
		ALLEGRO_FONT* powerFont;
        
        // Explosions
        std::vector<Explosion*> explosions;
		
        ALLEGRO_SAMPLE* enemy_dead_sample;
        ALLEGRO_SAMPLE_ID enemy_dead_sample_id;

        ALLEGRO_FONT* scoreFont;
        
        ////////// Level Completed Dialog ///////////////////
        ALLEGRO_BITMAP* dialog_box;
        Button* forwardButton;
        Button* restartButton;
        Button* homeButton;
        Button* pauseButton;
        
        std::vector<Powerup*> powerups;
        bool screenSwipe;
        float swipeX;
        float swipeY;
        std::vector<Bullet*> swipeBullets;
        
        bool gameOver;
        ALLEGRO_BITMAP* game_over_dialog;

        ALLEGRO_AUDIO_STREAM* backgroundMusic;
        
	};

#endif // PLAYSTATE_H
