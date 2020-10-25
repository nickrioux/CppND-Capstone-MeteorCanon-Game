#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

using std::string;

namespace FontManager {
    
        std::shared_ptr<TTF_Font> LoadFont(const string & fileName, int fontSize);
        void Draw(std::shared_ptr<SDL_Texture> texture, SDL_Rect position); 
};

#endif