#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>

namespace Collision {
    bool CheckRectCollision(const SDL_Rect & rectA, const SDL_Rect & rectB);   
};

#endif