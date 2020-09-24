#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include <vector>
#include "SDL.h"

class Sprite {

    public :

    Sprite(SDL_Texture * img,int,int);
    ~Sprite();

    void Render(SDL_Renderer *, SDL_Rect *);

    void SetPos(int x, int y) { _rect.x = x; _rect.y = y; }
    int GetX() { return(_rect.x); }
    int GetY() { return(_rect.y); }

    int GetSize() { return(_rect.w); }

    private:

        SDL_Texture * _img;
        SDL_Rect      _rect;
};

#endif