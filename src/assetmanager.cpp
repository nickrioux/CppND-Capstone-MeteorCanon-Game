#include <iostream>

#include "assetmanager.h"

#include "texturemanager.h"
#include "fontmanager.h"
#include "components/transformcomponent.h"

void AssetManager::ClearData() {
    textures_.clear();
    fonts_.clear();
}

void AssetManager::AddTexture(std::string textureId, const std::string & filePath) {
    textures_.emplace(textureId, TextureManager::LoadTexture(filePath.c_str()));
}

void AssetManager::AddFont(std::string fontId, const std::string & filePath, int fontSize) {
    fonts_.emplace(fontId, FontManager::LoadFont(filePath.c_str(), fontSize));
}

std::shared_ptr<SDL_Texture> AssetManager::GetTexture(std::string textureId) {
    return textures_[textureId];
}

std::shared_ptr<TTF_Font> AssetManager::GetFont(std::string fontId) {
    return fonts_[fontId];
}