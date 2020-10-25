#include <SDL.h>
#include <SDL_image.h>

#include "game.h"

#include "texturemanager.h"


std::shared_ptr<SDL_Texture> TextureManager::LoadTexture(const string & fileName) {
    SDL_Surface* surface = IMG_Load(fileName.c_str());
    std::shared_ptr<SDL_Texture> texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(Game::GetGame().GetRenderer().get(), surface),SDL_DestroyTexture);
    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::Draw(std::shared_ptr<SDL_Texture> texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::GetGame().GetRenderer().get(), texture.get(), &sourceRectangle, &destinationRectangle, 0.0, NULL, flip);
}

void TextureManager::GetInfo(std::shared_ptr<SDL_Texture> texture, int & w, int &h) {
    SDL_QueryTexture(texture.get(), NULL, NULL, &w, &h);
}