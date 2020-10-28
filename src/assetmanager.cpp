#include "assetmanager.h"

#include "texturemanager.h"
#include "fontmanager.h"
#include "components/transformcomponent.h"

AssetManager::AssetManager() {

}

AssetManager::~AssetManager() {

}

void AssetManager::ClearData() {
    _textures.clear();
}

void AssetManager::AddTexture(std::string textureId, const std::string & filePath) {
    _textures.emplace(textureId, TextureManager::LoadTexture(filePath.c_str()));
}

void AssetManager::AddFont(std::string fontId, const std::string & filePath, int fontSize) {
    _fonts.emplace(fontId, FontManager::LoadFont(filePath.c_str(), fontSize));
}

std::shared_ptr<SDL_Texture> AssetManager::GetTexture(std::string textureId) {
    return _textures[textureId];
}

std::shared_ptr<TTF_Font> AssetManager::GetFont(std::string fontId) {
    return _fonts[fontId];
}