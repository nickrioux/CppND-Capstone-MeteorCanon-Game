#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

namespace FontManager {
    
        std::shared_ptr<TTF_Font> LoadFont(const std::string & fileName, int fontSize);
        void Draw(std::shared_ptr<SDL_Texture> texture, SDL_Rect position); 
};

#endif
