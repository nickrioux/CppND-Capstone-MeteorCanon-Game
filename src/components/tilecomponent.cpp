#include "tilecomponent.h"

TileComponent::TileComponent(int srcRectX, int srcRectY, int x, int y, int tileSize, int tileScale, const string & textureId) {
    _texture = Game::GetGame().GetAssetManager().GetTexture(textureId);
    _srcRect.x = srcRectX;
    _srcRect.y = srcRectY;
    _srcRect.w = tileSize;
    _srcRect.h = tileSize;

    _destRect.x = x;
    _destRect.y = y;
    _destRect.w = tileSize * tileScale;
    _destRect.h = tileSize * tileScale;

    _position.x = x;
    _position.y = y;
}

void TileComponent::Update(float deltaTime) {
    _destRect.x = _position.x - Game::GetGame().GetCamera().x;
    _destRect.y = _position.y - Game::GetGame().GetCamera().y;
}

void TileComponent::Render() {
    TextureManager::Draw(_texture, _srcRect, _destRect, SDL_FLIP_NONE);
}