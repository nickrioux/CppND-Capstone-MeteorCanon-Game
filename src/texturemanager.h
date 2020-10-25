#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <SDL.h>

using std::string;

namespace TextureManager {
        std::shared_ptr<SDL_Texture> LoadTexture(const string & fileName);
        void Draw(std::shared_ptr<SDL_Texture> texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip);
        void GetInfo(std::shared_ptr<SDL_Texture> texture, int & w, int & h);
};

#endif