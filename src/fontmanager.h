#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

using std::string;

class FontManager {
    public:

        static TTF_Font* LoadFont(string fileName, int fontSize);
        static void Draw(SDL_Texture* texture, SDL_Rect position); 
};

#endif