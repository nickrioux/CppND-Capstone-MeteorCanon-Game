#include <SDL_image.h>

#include "game.h"

#include "texturemanager.h"


SDL_Texture* TextureManager::LoadTexture(const string & fileName) {
    SDL_Surface* surface = IMG_Load(fileName.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetGame().GetRenderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::GetGame().GetRenderer(), texture, &sourceRectangle, &destinationRectangle, 0.0, NULL, flip);
}

void TextureManager::GetInfo(SDL_Texture * texture, int & w, int &h) {
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}