cmake_minimum_required(VERSION 3.4.1)

add_library(native-lib
        SHARED
        main.cpp Game.cpp State.cpp
        MenuState.cpp PlayState.cpp Background.cpp LevelSelectState.cpp Button.cpp GameObject.cpp Player.cpp Bullet.cpp Enemy.cpp Explosion.cpp Powerup.cpp PauseState.cpp SplashState.cpp)

set(NATIVE_LIB native-lib)
set(JNI_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/..)
include(allegro.cmake)
