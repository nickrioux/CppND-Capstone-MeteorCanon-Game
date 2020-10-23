#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

using std::string;

namespace FontManager {
    
        TTF_Font* LoadFont(string fileName, int fontSize);
        void Draw(SDL_Texture* texture, SDL_Rect position); 
};

#endif