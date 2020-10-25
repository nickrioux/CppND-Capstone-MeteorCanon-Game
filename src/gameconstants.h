#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <string>

#include <SDL.h>

namespace GameConstants { 

     enum GameState {
               GAME_STATE_RUNNING,
               GAME_STATE_GAMEOVER
     };

     enum CollisionType {
          NO_COLLISION,
          PLAYER_METEOR_COLLISION,
          PLAYER_MISSILE_COLLISION,
          PLAYER_SPINNER_COLLISION,
          PLAYER_EXPLOSION_COLLISION,
          ENEMY_BULLET_COLLISION,
          ENEMY_EXPLOSION_COLLISION,
          PLAYER_LEVEL_COMPLETE_COLLISION,
          PLAYER_LEFT_BOUNDARY_COLLISION,
          PLAYER_RIGHT_BOUNDARY_COLLISION,
          METEOR_GROUND_COLLISION,
          FALLING_OBJECT_BOUNDARY_COLLISION,
          SPINNER_GROUND_COLLISION
     };

     enum LayerType {
          BACKGROUND_LAYER = 0,
          MOUNTAIN_LAYER = 1,
          NPC_LAYER = 2,
          PLAYER_LAYER = 3,
          BULLET_LAYER = 4,
          UI_LAYER = 5
     };

     enum LevelsScore {
     #if 1
          LEVEL_1 = 0,
          LEVEL_2 = 1000,
          LEVEL_3 = 5000,
          LEVEL_4 = 20000,
          LEVEL_5 = 50000,
          LEVEL_6 = 1000000
     #else
          LEVEL_1 = 0,
          LEVEL_2 = 200,
          LEVEL_3 = 300,
          LEVEL_4 = 600,
          LEVEL_5 = 800,
          LEVEL_6 = 1000
     #endif
     };

     enum Score {
          SCORE_LOST_BASE = -100,
          SCORE_BIG_ROCK_LANDING = -5,
          SCORE_SMALL_ROCK_LANDING = -10,
          SCORE_BIG_ROCK = 10,
          SCORE_SMALL_ROCK = 20,
          SCORE_BIG_SPINNER = 40,
          SCORE_SMALL_SPINNER = 80,
          SCORE_PULSAR = 50,
          SCORE_SAUCER_BULLET = 50,
          SCORE_SAUCER = 100,
          SCORE_LIFE = 1000
     };

     enum Speed {
          PLAYER_SPEED = 300,
          BULLET_SPEED = 350,
          METEOR_SPEED = 100,
          UFO_SPEED = 100,
          SPINNER_SPEED = 100,
          CANON_EXPLOSION_LOW_SPEED = 300,
          CANON_EXPLOSION_HIGH_SPEED = 800
     };

     enum Size {
          BIG_METEOR_W = 60,
          BIG_METEOR_H = 35,
          SMALL_METEOR_W = 30,
          SMALL_METEOR_H = 18
     };

     const unsigned int NUM_LAYERS = 6;

     const SDL_Color WHITE_COLOR = {255,255,255,255};

     //Entity Types
     enum EntityType {
          PLAYER_ENTITY,
          BIG_METEOR_ENTITY,
          SMALL_METEOR_ENTITY,
          TILE_ENTITY,
          COLLIDE_ENTITY,
          UFO_ENTITY,
          BIG_SPINNER_ENTITY,
          SMALL_SPINNER_ENTITY,
          MISSILE_ENTITY,
          BULLET_ENTITY,
          UI_ENTITY,
          EXPLOSION_ENTITY
     };

     //Entity Names
     const std::string BIG_METEOR_ENTITY_NAME = {"BIG_METEOR"};
     const std::string SMALL_METEOR_ENTITY_NAME = {"SMALL_METEOR"};
     const std::string SMALL_SPINNER_ENTITY_NAME = {"SMALL_SPINNER"};
     const std::string BIG_SPINNER_ENTITY_NAME = {"BIG_SPINNER"};

     //ENTITY LIFETIME
     const int ENTITY_LIFE_INFINITE = -1.0;
     const int ENTITY_LIFE_EXPLOSION = 2;

     //Scale
     const float SCALE_GAME = {1.0f};

     //Map Size
     const int MAP_TILE_SIZE = 64;
     const int MAP_WIDTH = 16;
     const int MAP_HEIGTH = 12;

     //Times Constant
     const float TIME_RESTART_PLAYER = 2.0f;
     const float TIME_FREQ_METEOR = 2.0f;
     const float TIME_FREQ_SPINNER = 10.0f;

     //Speed Factor
     const float SPEED_FACTOR_KILL_TRANSITION = 0.10f;
     const float SPEED_FACTOR_LEVEL1 = 1.0f;
     const float SPEED_FACTOR_LEVEL2 = 2.0f;
     const float SPEED_FACTOR_LEVEL3 = 3.0f;
     const float SPEED_FACTOR_LEVEL4 = 4.0f;
     const float SPEED_FACTOR_LEVEL5 = 5.0f;
     const float SPEED_FACTOR_LEVEL6 = 6.0f;
};

#endif