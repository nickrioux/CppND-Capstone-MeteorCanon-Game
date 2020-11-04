#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <memory>
#include <string>
#include <SDL.h>

namespace TextureManager {
        std::shared_ptr<SDL_Texture> LoadTexture(const std::string & fileName);
        void Draw(std::shared_ptr<SDL_Texture> texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip);
        void GetInfo(std::shared_ptr<SDL_Texture> texture, int & w, int & h);
};

#endif
