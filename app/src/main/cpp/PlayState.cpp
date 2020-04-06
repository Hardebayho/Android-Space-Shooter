#include "PlayState.h"
#include <allegro5/allegro_primitives.h>
#include <string>
#include "Game.h"
#include "MenuState.h"
#include "PauseState.h"

PlayState::PlayState(Game& game_, int _level, int currentPlayerBulletDamage_) : State(game_), level(_level), currentPlayerBulletDamage(currentPlayerBulletDamage_) {}

void PlayState::init() {
	bg = new Background("res/bg1.jpg", 4);
    std::string levelPath("res/level");
    levelPath.append(std::to_string(level)).append(".cfg");
    level_config = al_load_config_file(levelPath.c_str());
    assert(level_config);
    
    // Initialize all these first
    num_enemies = 0;
    num_acts = 0;
    
    srand(time(nullptr));
    
    // Load the enemies based on the info from the config file
    level_name = al_get_config_value(level_config, "", "Level Name");
    num_enemies = std::stoi(al_get_config_value(level_config, "", "Enemies"));
    num_acts = std::stoi(al_get_config_value(level_config, "", "Acts"));
    
    currentAct = 0;
    dead_enemies = 0;
    spawnEnemies();
    
    player.init();
    player.setBulletDamage(currentPlayerBulletDamage);
    
    enemy_dead_sample = al_load_sample("res/enemy_dead.wav");
    
    level_start = false;
    level_title_font = al_load_font("res/astron-boy-video.ttf", 45, 0);
    assert(level_title_font);
    
    level_title_x = GAME_WIDTH;
    level_title_y = GAME_HEIGHT / 2 - al_get_font_line_height(level_title_font) / 2;
    
    level_start_timer = al_create_timer(2.0);
    level_start_event_queue = al_create_event_queue();
    al_register_event_source(level_start_event_queue, al_get_timer_event_source(level_start_timer));
    
    finishAnimation = false;
    
    level_finished_timer = 0;
    level_finished_timer_diff = 2;
    levelCompleted = false;
    
    dialog_box = al_load_bitmap("res/level_complete_dialog.png");
    
    forwardButton = new Button("res/forward_button.png", 70, GAME_HEIGHT / 2 + 30);
    restartButton = new Button("res/restart_button.png", 160, GAME_HEIGHT / 2 + 30);
    homeButton = new Button("res/home_button.png", 250, GAME_HEIGHT / 2 + 30);
    pauseButton = new Button("res/pause_button.png", GAME_WIDTH - 80, 20);
    
    scoreFont = al_load_font("res/titleFont.ttf", 20, 0);
    assert(scoreFont);
    
    screenSwipe = false;
    swipeX = 0;
    swipeY = 0;
    
    gameOver = false;
    game_over_dialog = al_load_bitmap("res/game_over_dialog.png");

    backgroundMusic = al_load_audio_stream("res/ingame_music.ogg", 2, 2048);
    assert(backgroundMusic);
    al_set_audio_stream_playmode(backgroundMusic, ALLEGRO_PLAYMODE_LOOP);

    al_attach_audio_stream_to_mixer(backgroundMusic, al_get_default_mixer());

    powerFont = al_load_font("res/titleFont.ttf", 18, 0);
    assert(powerFont);
}

void PlayState::update() {
    
    // Check if any of the buttons are clicked

    if (pauseButton->isClicked()) {
        pause();
    }
    
    if (gameOver || levelCompleted) {
        
        if (gameOver) { 
            forwardButton->setX(-500);
            forwardButton->setY(-500);
        }
        
        if (forwardButton->isClicked()) {
            game.changeState(new PlayState(game, ++level, player.getBulletDamage()));
        } else if (restartButton->isClicked()) {
            int currentPlayerBulletDamage = player.getBulletDamage();
            dispose();
            init();
            player.setBulletDamage(currentPlayerBulletDamage);
        } else if (homeButton->isClicked()) {
            game.changeState(new MenuState(game));
        }
        
        return;
    }
    
    if (currentAct >= num_acts && level_finished_timer == 0) {
        // Level has been completed, wait for a second
        level_finished_timer = al_get_time();
    }
    
    if (!levelCompleted && level_finished_timer && al_get_time() - level_finished_timer >= level_finished_timer_diff) {
        levelCompleted = true;
        return;
    }
    
    // if level is completed, don't update the rest of the game
    if (levelCompleted) return;
    
    if (player.isDead()) {
        gameOver = true;
    }
    
    bg->update();
    
    if (!level_start) {
        
        if (al_get_next_event(level_start_event_queue, &level_start_event)) {
            al_stop_timer(level_start_timer);
            finishAnimation = true;
        }
        
        if (finishAnimation) {
            level_title_x -= 2.2f;
            
            // This is where we start the level
            if (level_title_x < -GAME_WIDTH) level_start = true;
            return;
        }
        
        int x, y, width, height;
        al_get_text_dimensions(level_title_font, level_name.c_str(), &x, &y, &width, &height);
        
        if (level_title_x > GAME_WIDTH / 2 - width / 2) {
            level_title_x -= 2.2f;
        } else {
            if (!al_get_timer_started(level_start_timer)) al_start_timer(level_start_timer);
        }
        
        return;
    }
    
	player.update();
    
    for (auto& enemy : enemies) {
        enemy->update();
    }
    
    // Do we need to spawn new enemies
    if (enemies.empty() && currentAct < num_acts) {
        currentAct++;
        spawnEnemies();
    }
    
    // Update the explosions
    for (auto& explosion : explosions) {
        explosion->update();
    }
    
    // Update the powerups
    for (auto& powerup : powerups) {
        powerup->update();
    }
    
    // Update swipe bullets
    if (screenSwipe) {
        if (swipeBullets.empty()) screenSwipe = false;
        for (auto& swipeBullet : swipeBullets) {
            swipeBullet->update();
        }
    }
    
    //////////////////////// Collision detection ////////////////////////////
    
    // Player bullets - Enemies
    for (auto& bullet : player.getBullets()) {
        for (auto& enemy : enemies) {
            if (bullet->intersects(enemy) && !enemy->isDead()) {
                enemy->loseLife(bullet->getDamage());
                
                if (enemy->isDead()) score += enemy->getRank();
                
                // Add explosion effect for the bullet-enemy collision
                explosions.push_back(new Explosion(0.1f, al_map_rgb_f(1, 0, 0), bullet->getX(), bullet->getY()));
                bullet->remove();
            }
        }
    }
    
    // Powerups - Player
    for (auto& powerup : powerups) {
        if (player.intersects(powerup)) {
            
            int type = powerup->getType();
            
            switch (type) {
            case 1:
                player.increaseBulletDamage();
                score += 50;
                break;
            case 2:
                screenSwipe = true;
                swipeX = player.getX();
                swipeY = player.getY();
                
                for (int i = 0; i < 100; i++) {
                    float angle = (rand() % 361);
                    float speed = 2 + rand() % 10;
                    Bullet* b = new Bullet(10, "res/player_normal_bullet.png", angle, swipeX, swipeY);
                    b->setSpeed(speed);
                    swipeBullets.push_back(b);
                }
                
                break;
            }
            
            powerup->removeNow();
        }
    }
    
    // Player - Enemies
    for (auto& enemy : enemies) {
        if (player.intersects(enemy)) {
            player.die();
        }
    }
    
    // Player - Enemy bullets
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy->getBullets()) {
            if (player.intersects(bullet)) {
                player.die();
            }
        }
    }
    
    // Swipe Bullets - Enemies
    for (auto& sb : swipeBullets) {
        for (auto& enemy : enemies) {
            if (sb->intersects(enemy)) {
                enemy->die();
                sb->remove();
                score += 50;
            }
        }
    }
    
    // Check for dead enemies
    for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++) {
        if ((*enemy)->isDead()) {
            explosions.push_back(new Explosion(1, al_map_rgb_f(1, 0, 1), (*enemy)->getX(), (*enemy)->getY()));
            
            // Chance for powerup
            int randNumber = rand() % 50;
            
            // From one to three
            if (randNumber > 0 && randNumber <= 2)
                powerups.push_back(new Powerup(randNumber, (*enemy)->getX(), (*enemy)->getY()));
            
            (*enemy)->dispose();
            delete (*enemy);
            enemies.erase(enemy);
            enemy--;
            al_play_sample(enemy_dead_sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &enemy_dead_sample_id);
            
        }
    }
    
    // Check for removable explosions
    for (auto explosion = explosions.begin(); explosion != explosions.end(); explosion++) {
        if ((*explosion)->isDone()) {
            (*explosion)->dispose();
            delete (*explosion);
            explosions.erase(explosion);
            explosion--;
        }
    }
    
    // Check for removable powerups
    for (auto powerup = powerups.begin(); powerup != powerups.end(); powerup++) {
        if ((*powerup)->isDead()) {
            (*powerup)->dispose();
            delete (*powerup);
            powerups.erase(powerup);
            powerup--;
        }
    }
    
    // Check for removable swipe bullets
    for (auto swipeBullet = swipeBullets.begin(); swipeBullet != swipeBullets.end(); swipeBullet++) {
        if ((*swipeBullet)->isDead()) {
            (*swipeBullet)->dispose();
            delete (*swipeBullet);
            swipeBullets.erase(swipeBullet);
            swipeBullet--;
        }
    }
    
    if (score > 10000) score = 10000;
}

void PlayState::render() {
    
	bg->render();
	player.render();
    
    for (auto& enemy : enemies) {
        enemy->render();
    }
    
    for (auto& explosion: explosions) {
        explosion->render();
    }
    
    for (auto& powerup : powerups) {
        powerup->render();
    }
    
    if (screenSwipe) {
        for (auto& swipeBullet : swipeBullets) {
            swipeBullet->render();
        }
    }
    
    // Draw level text if the level has not started
    if (!level_start) {
        al_draw_text(level_title_font, al_map_rgb_f(1, 0.3f, 0.7f), level_title_x, level_title_y, 0, level_name.c_str());
    }
    
    if (levelCompleted || gameOver) {
        // Draw the level completed dialog
        float dialog_x, dialog_y, dialog_width, dialog_height;
        dialog_width = al_get_bitmap_width(dialog_box);
        dialog_height = al_get_bitmap_height(dialog_box);
        dialog_x = GAME_WIDTH / 2 - dialog_width / 2;
        dialog_y = GAME_HEIGHT / 2 - dialog_height / 2;
        al_draw_bitmap(gameOver ? game_over_dialog : dialog_box, dialog_x, dialog_y, 0);
        
        if (gameOver) restartButton->setX(70);
        
        if (levelCompleted) forwardButton->render();
        restartButton->render();
        homeButton->render();
        
        // Draw score text
        int x, y, width, height;

        al_get_text_dimensions(scoreFont, "Score: 100", &x, &y, &width, &height);
        al_draw_textf(scoreFont, al_map_rgb_f(1, 1, 1), GAME_WIDTH / 2 - width / 2 - 5, GAME_HEIGHT / 2 - height / 2 - 30, 0, "Score: %i", score);
        
    }

    al_draw_textf(powerFont, al_map_rgb_f(0, 1, 0), 20, 20, 0, "Bullet Power: ");
    for (int i = 0; i < player.getBulletDamage(); i++) {
        al_draw_filled_rectangle(40 + i * 30, 40, 60 + i * 30, 60, al_map_rgb_f(0, 0.2, 1));
    }

    pauseButton->render();
    
}

void PlayState::handleInput(ALLEGRO_EVENT &event) {
	player.handleInput(event);
    forwardButton->update(event);
    restartButton->update(event);
    homeButton->update(event);
    pauseButton->update(event);
}

void PlayState::dispose() {
	bg->dispose();
	delete bg;
	player.dispose();
    
    for (auto& enemy : enemies) {
        enemy->dispose();
        delete enemy;
    }
    enemies.clear();
    
    for (auto& explosion : explosions) {
        explosion->dispose();
        delete explosion;
    }
    explosions.clear();
    
    al_destroy_config(level_config);
    al_destroy_sample(enemy_dead_sample);
    
    al_destroy_event_queue(level_start_event_queue);
    al_destroy_timer(level_start_timer);
    
    al_destroy_font(level_title_font);
    al_destroy_font(scoreFont);
    al_destroy_font(powerFont);
    
    forwardButton->dispose();
    delete forwardButton;
    restartButton->dispose();
    delete restartButton;
    homeButton->dispose();
    delete homeButton;
    pauseButton->dispose();
    delete pauseButton;
    
    for (auto& powerup : powerups) {
        powerup->dispose();
        delete powerup;
    }
    powerups.clear();
    
    for (auto& swipeBullet : swipeBullets) {
        swipeBullet->dispose();
        delete swipeBullet;
    }
    swipeBullets.clear();
    
    
    al_destroy_bitmap(dialog_box);
    al_destroy_bitmap(game_over_dialog);

    al_destroy_audio_stream(backgroundMusic);
}

void PlayState::spawnEnemies() {
    if (currentAct < num_acts) {
        // Spawn new enemies
        std::string text("Act "); 
        text.append(std::to_string(currentAct + 1));
        int num_act_enemies = std::stoi(al_get_config_value(level_config, text.c_str(), "num_enemies"));
        int rank = std::stoi(al_get_config_value(level_config, text.c_str(), "ranks"));
        for (int i = 0; i < num_act_enemies; i++) {
            enemies.emplace_back(new Enemy(rank));
        }
    }
}

void PlayState::pause() {
    al_flush_event_queue(level_start_event_queue);
    al_pause_event_queue(level_start_event_queue, true);
    
    player.pause();
    for (auto& explosion : explosions) {
        explosion->pause();
    }
    
    game.pauseState(new PauseState(game));

    al_set_audio_stream_playing(backgroundMusic, false);
}

void PlayState::resume() {
    al_pause_event_queue(level_start_event_queue, false);
    
    player.resume();
    for (auto& explosion : explosions) {
        explosion->resume();
    }
    al_set_audio_stream_playing(backgroundMusic, true);
}
