#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL.h>

#include "glm/glm.hpp" 

#include "game.h"
#include "assetmanager.h"
#include "texturemanager.h"

#include "component.h"

class TileComponent : public Component {
    public :
        TileComponent(int srcRectX, int srcRectY, int x, int y, int tileSize, int tileScale, string textureId) {
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

        ~TileComponent() {
            SDL_DestroyTexture(_texture);
        }

        void Update(float deltaTime) override {
            _destRect.x = _position.x - Game::GetGame().GetCamera().x;
            _destRect.y = _position.y - Game::GetGame().GetCamera().y;
        }

        void Render() override {
            TextureManager::Draw(_texture, _srcRect, _destRect, SDL_FLIP_NONE);
        }

    private:
        SDL_Texture * _texture;
        SDL_Rect _srcRect;
        SDL_Rect _destRect;
        glm::vec2 _position;
};

#endif