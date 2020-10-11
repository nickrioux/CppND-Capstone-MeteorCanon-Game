#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>

#include <SDL.h>

#include "texturemanager.h"
#include "transformcomponent.h"

using std::string;

class SpriteComponent: public Component {
    public:
        SpriteComponent(const string & filePath, bool isFixed = false) : _transform(nullptr), _isFixed(isFixed) {
            SetTexture(filePath);
        }

        void SetTexture(std::string assetTextureId) {
            _texture = Game::GetGame()->GetAssetManager()->GetTexture(assetTextureId);
        }

        void Init() override {
            //Retrieve Texture Size
            int w,h;
            TextureManager::GetInfo(_texture,w,h);
            _transform = GetOwner()->GetComponent<TransformComponent>();
            _transform->SetWidth(w);
            _transform->SetHeight(h);
            _sourceRectangle.x = 0;
            _sourceRectangle.y = 0;
            _sourceRectangle.w = _transform->GetWidth();
            _sourceRectangle.h = _transform->GetHeight();
        }

        void Update(float deltaTime) override {
            _destinationRectangle.x = static_cast<int>(_transform->GetPosition().x) - (_isFixed ? 0 : Game::GetGame()->GetCamera()->x);
            _destinationRectangle.y = static_cast<int>(_transform->GetPosition().y) - (_isFixed ? 0 : Game::GetGame()->GetCamera()->y);

            _destinationRectangle.w = _transform->GetWidth() * _transform->GetScale();
            _destinationRectangle.h = _transform->GetHeight() * _transform->GetScale();
        }

        void Render() override {
            TextureManager::Draw(_texture, _sourceRectangle, _destinationRectangle, _spriteFlip);
        }

        private:
            bool _isFixed{false};
            SDL_RendererFlip _spriteFlip = SDL_FLIP_NONE;
            TransformComponent * _transform;
            SDL_Texture* _texture;
            SDL_Rect _sourceRectangle;
            SDL_Rect _destinationRectangle;
};

#endif