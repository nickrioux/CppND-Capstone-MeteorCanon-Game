#include "tilecomponent.h"

TileComponent::TileComponent(int srcRectX, int srcRectY, int x, int y, int tileSize, int tileScale, const std::string & textureId) 
                : texture_{Game::GetGame().GetAssetManager().GetTexture(textureId)}, 
                srcRect_{srcRectX,srcRectY,tileSize,tileSize},
                destRect_{x,y,tileSize*tileScale,tileSize*tileScale},
                position_{x,y} {
    
}

void TileComponent::Update(float deltaTime) {
    destRect_.x = position_.x - Game::GetGame().GetCamera().x;
    destRect_.y = position_.y - Game::GetGame().GetCamera().y;
}

void TileComponent::Render() {
    TextureManager::Draw(texture_, srcRect_, destRect_, SDL_FLIP_NONE);
}