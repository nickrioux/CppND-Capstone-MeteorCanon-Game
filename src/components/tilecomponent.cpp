#include "tilecomponent.h"

TileComponent::TileComponent(int srcRectX, int srcRectY, int x, int y, int tileSize, int tileScale, const std::string & textureId) : _texture{Game::GetGame().GetAssetManager().GetTexture(textureId)}, 
                                                                                                                                _srcRect{srcRectX,srcRectY,tileSize,tileSize},
                                                                                                                                _destRect{x,y,tileSize*tileScale,tileSize*tileScale},
                                                                                                                                _position{x,y} {
    
}

void TileComponent::Update(float deltaTime) {
    _destRect.x = _position.x - Game::GetGame().GetCamera().x;
    _destRect.y = _position.y - Game::GetGame().GetCamera().y;
}

void TileComponent::Render() {
    TextureManager::Draw(_texture, _srcRect, _destRect, SDL_FLIP_NONE);
}