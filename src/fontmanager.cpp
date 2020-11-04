#include "game.h"

#include "fontmanager.h"

std::shared_ptr<TTF_Font> FontManager::LoadFont(const std::string & fileName, int fontSize) {
    return std::shared_ptr<TTF_Font>(TTF_OpenFont(fileName.c_str(), fontSize),TTF_CloseFont);
}

void FontManager::Draw(std::shared_ptr<SDL_Texture> texture, SDL_Rect position) {
    SDL_RenderCopy(Game::GetGame().GetRenderer().get(), texture.get(), NULL, &position);
}
