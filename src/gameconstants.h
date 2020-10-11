#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <SDL.h>

namespace GameConstants { 
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

     const unsigned int NUM_LAYERS = 6;

     const SDL_Color WHITE_COLOR = {255,255,255,255};
};

#endif