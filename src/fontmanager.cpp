#include "game.h"

#include "fontmanager.h"

TTF_Font* FontManager::LoadFont(string fileName, int fontSize) {
    return TTF_OpenFont(fileName.c_str(), fontSize);
}

void FontManager::Draw(SDL_Texture *texture, SDL_Rect position) {
    SDL_RenderCopy(Game::GetGame().GetRenderer(), texture, NULL, &position);
}
