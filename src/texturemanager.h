#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <SDL.h>

using std::string;

namespace TextureManager {
        SDL_Texture* LoadTexture(const string & fileName);
        void Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip);
        void GetInfo(SDL_Texture * texture, int & w, int & h);
};

#endif