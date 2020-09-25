#include "SDL.h"
#include "sprite.h"

Sprite::Sprite(SDL_Texture * img, int w, int h,float gridW, float gridH) : _img(img), _gridSizeW(gridW), _gridSizeH(gridH) {
        _rect.x = 0;
        _rect.y = 0;
        _rect.w = w;
        _rect.h = h;
}

Sprite::~Sprite() {};

void Sprite::Render(SDL_Renderer * renderer, SDL_Rect * dest) {
    SDL_RenderCopyEx(renderer, _img, &_rect, dest, 0, NULL, SDL_FLIP_NONE);
}

