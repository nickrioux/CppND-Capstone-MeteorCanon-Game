#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include <vector>
#include "SDL.h"

class Sprite {

    public :

    Sprite(SDL_Texture * img,int,int,float,float);
    ~Sprite();

    void Render(SDL_Renderer *, SDL_Rect *);

    void SetPos(int x, int y) { _rect.x = x; _rect.y = y; }
    int GetX() { return(_rect.x); }
    int GetY() { return(_rect.y); }

    float GetGridSizeW() { return(_gridSizeW); }
    float GetGridSizeH() { return(_gridSizeH); }

    private:

        SDL_Texture * _img;
        SDL_Rect      _rect;
        float _gridSizeW;
        float _gridSizeH;
};

#endif