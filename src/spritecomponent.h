#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <iostream>
#include <string>

#include <SDL.h>

#include "texturemanager.h"
#include "transformcomponent.h"

using std::string;

class SpriteComponent: public Component {
    public:
        SpriteComponent(const string & filePath, bool isFixed = false) : _transform(nullptr), _isFixed(isFixed), _animationSpeed(0), _isAnimated(false), _numFrames(0) {
            SetTexture(filePath);
        }

        SpriteComponent(std::string id, int numFrames, int animationSpeed, bool isFixed = false) : _isAnimated(true), _numFrames(numFrames), _animationSpeed(animationSpeed), _isFixed(isFixed) {
            SetTexture(id);
        }

        void SetTexture(std::string assetTextureId) {
            std::cout << "Texture ID : " + assetTextureId << std::endl;
            _texture = Game::GetGame().GetAssetManager().GetTexture(assetTextureId);

            if (_texture == nullptr)
            {
                    std::cout << "ERROR TEXTURE ID" << std::endl;
            }
        }

        void Init() override {
            _transform = GetOwner()->GetComponent<TransformComponent>();
            _sourceRectangle.x = 0;
            _sourceRectangle.y = 0;
            _sourceRectangle.w = _transform->GetWidth();
            _sourceRectangle.h = _transform->GetHeight();
        }

        void Update(float deltaTime) override {

            if (_isAnimated) {
                _sourceRectangle.x = _sourceRectangle.w * static_cast<int>((SDL_GetTicks() / _animationSpeed) % _numFrames);
            }

            _destinationRectangle.x = static_cast<int>(_transform->GetPosition().x) - (_isFixed ? 0 : Game::GetGame().GetCamera().x);
            _destinationRectangle.y = static_cast<int>(_transform->GetPosition().y) - (_isFixed ? 0 : Game::GetGame().GetCamera().y);

            _destinationRectangle.w = _transform->GetWidth() * _transform->GetScale();
            _destinationRectangle.h = _transform->GetHeight() * _transform->GetScale();
        }

        void Render() override {
            TextureManager::Draw(_texture, _sourceRectangle, _destinationRectangle, _spriteFlip);
        }

        private:
            bool _isFixed{false};
            SDL_RendererFlip _spriteFlip = SDL_FLIP_NONE;
            std::shared_ptr<TransformComponent> _transform;
            std::shared_ptr<SDL_Texture> _texture;
            SDL_Rect _sourceRectangle;
            SDL_Rect _destinationRectangle;

            bool _isAnimated{false};
            int _numFrames;
            int _animationSpeed;
};






#endif